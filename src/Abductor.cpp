#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Abductor.h"



void limit(sf::Vector2f& v, float max)
{
	if (Helpers::getLength(v) > max)
	{
		v = Helpers::normaliseCopy(v) * max;
	}
}

Abductor::Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, bool predCheck)
	: AI(Type::Abductor, startPos, worldSize)
{
	predator = predCheck;
	if (predCheck == true) {
		maxSpeed = 7.5;
		maxForce = 0.5;
		velocity = sf::Vector2f(rand() % 3 - 1, rand() % 3 - 1);
	}
	else {
		maxSpeed = 3.5;
		maxForce = 0.5;
		velocity = sf::Vector2f(rand() % 3 - 2, rand() % 3 - 2); // Allows for range of -2 -> 2
	}
	acceleration = sf::Vector2f(0, 0);
	location = startPos;
}

// =============================================== //
// ======== Abductor Functions from Abductor.h =========== //
// =============================================== //

// Adds force sf::Vector2f to current force sf::Vector2f
void Abductor::applyForce(sf::Vector2f force)
{
	acceleration += force;
}

// Function that checks and modifies the distance
// of a Abductor if it breaks the law of separation.
sf::Vector2f Abductor::Separation(const std::vector<Abductor>& Abductors)
{
	// If the Abductor we're looking at is a predator, do not run the separation
	// algorithm
	
	// Distance of field of vision for separation between Abductors
	float desiredseparation = 100;

	sf::Vector2f steer(0, 0);
	int count = 0;
	// For every Abductor in the system, check if it's too close
	for (int i = 0; i < Abductors.size(); i++)
	{
		// Calculate distance from current Abductor to Abductor we're looking at
		float d = Helpers::getLength(location - Abductors[i].location);
		// If this is a fellow Abductor and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			sf::Vector2f diff(0,0);
			diff = location - Abductors[i].location;
			Helpers::normalise(diff);
			diff /= d;      // Weight by distance
			steer += diff;
			count++;
		}
		// If current Abductor is a predator and the Abductor we're looking at is also
		// a predator, then separate only slightly 
		if ((d > 0) && (d < desiredseparation) && predator == true && Abductors[i].predator == true)
		{
			sf::Vector2f pred2pred(0, 0);
			pred2pred = location - Abductors[i].location;
			Helpers::normalise(pred2pred);
			pred2pred /= d;
			steer += pred2pred;
			count++;
		} 
		// If current Abductor is not a predator, but the Abductor we're looking at is
		// a predator, then create a large separation sf::Vector2f
		else if ((d > 0) && (d < desiredseparation+70) && Abductors[i].predator == true)
		{
			sf::Vector2f pred(0, 0);
			pred = location - Abductors[i].location;
			pred *= 900.f;
			steer += pred;
			count++;
		}
	}
	// Adds average difference of location to acceleration
	if (count > 0)
		steer /= (float)count;
	if (Helpers::getLength(steer) > 0) 
	{
		// Steering = Desired - Velocity
		Helpers::normalise(steer);
		steer *= maxSpeed;
		steer -= velocity;
		limit(steer, maxForce);
	}
	return steer;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Abductor passed as parameter to adjust to that
// of nearby Abductors.
sf::Vector2f Abductor::Alignment(const std::vector<Abductor>& Abductors)
{
	// If the Abductor we're looking at is a predator, do not run the alignment
	// algorithm
	//if (predator == true)
	//	return sf::Vector2f(0,0);
	float neighbordist = 500;

	sf::Vector2f sum(0, 0);	
	int count = 0;
	for (int i = 0; i < Abductors.size(); i++)
	{
		float d = Helpers::getLength(location - Abductors[i].location);
		if ((d > 0) && (d < neighbordist)) // 0 < d < 50
		{
			sum += Abductors[i].velocity;
			count++;
		}
	}
	// If there are Abductors close enough for alignment...
	if (count > 0)
	{
		sum /= (float)count;// Divide sum by the number of close Abductors (average of velocity)
		Helpers::normalise(sum);	   		// Turn sum into a unit vector, and
		sum *= maxSpeed;    // Multiply by maxSpeed
		// Steer = Desired - Velocity
		sf::Vector2f steer; 
		steer = sum - velocity; //sum = desired(average)  
		limit(steer, maxForce);
		return steer;
	} else {
		sf::Vector2f temp(0, 0);
		return temp;
	}
}

// Cohesion finds the average location of nearby Abductors and manipulates the 
// steering force to move in that direction.
sf::Vector2f Abductor::Cohesion(const std::vector<Abductor>& Abductors)
{
	// If the Abductor we're looking at is a predator, do not run the cohesion
	// algorithm
	//if (predator == true)
	//	return sf::Vector2f(0,0);

	float neighbordist = 500;

	sf::Vector2f sum(0, 0);	
	int count = 0;
	for (int i = 0; i < Abductors.size(); i++)
	{
		float d = Helpers::getLength(location - Abductors[i].location);
		if ((d > 0) && (d < neighbordist))
		{
			sum += Abductors[i].location;
			count++;
		}
	}
	if (count > 0)
	{
		sum /= (float)count;
		return seek(sum);
	} else {
		sf::Vector2f temp(0,0);
		return temp;
	}
}

// Seek function limits the maxSpeed, finds necessary steering force and
// normalizes the vectors.
sf::Vector2f Abductor::seek(const sf::Vector2f& v)
{
	sf::Vector2f desired;
	desired -= v;  // A vector pointing from the location to the target
	// Normalize desired and scale to maximum speed
	Helpers::normalise(desired);
	desired *= maxSpeed;
	// Steering = Desired minus Velocity
	acceleration = desired - velocity;
	limit(acceleration, maxForce);  // Limit to maximum steering force
	return acceleration;
}

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Abductor::update(float dt)
{
	//To make the slow down not as abrupt
	acceleration *= 0.4f;
	// Update velocity
	velocity += acceleration;
	// Limit speed
	limit(velocity, maxSpeed);
	location += velocity * dt;
	// Reset accelertion to 0 each cycle
	acceleration *= 0.f;

	m_position = location;
	m_sprite.setPosition(m_position);
}

//Run runs flock on the flock of Abductors for each Abductor.
//Which applies the three rules, modifies accordingly, updates data, checks is data is
//out of range, fixes that for SFML, and renders it on the window.
void Abductor::run(float dt, const std::vector<Abductor>& v)
{
	flock(v);
	update(dt);
}

//Applies all three laws for the flock of Abductors and modifies to keep them from
//breaking the laws.
void Abductor::flock(const std::vector<Abductor>& v) 
{
	sf::Vector2f sep = Separation(v);
	sf::Vector2f ali = Alignment(v);
	sf::Vector2f coh = Cohesion(v);
	// Arbitrarily weight these forces
	sep *= 1.5f;
	ali *= 1.0f; // Might need to alter weights for different characteristics
	coh *= 1.0f;
	// Add the force vectors to acceleration
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}

// Calculates the angle for the velocity of a Abductor which allows the visual 
// image to rotate in the direction that it is going in.
float Abductor::angle(sf::Vector2f v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / M_PI);
	return angle;
}

void Abductor::swarm(const std::vector<Abductor>& v)
{
	sf::Vector2f sum(0, 0);
	for (int i = 0; i < v.size(); i++)
	{
		if (this != &v[i]) 
		{
			float A = 0.2f;
			float N = 0.3f;
			float B = 0.3f;
			float M = 0.4f;

			sf::Vector2f	R;
			R = location - v[i].location;
			float D = Helpers::getLength(R);
			float U = (-A / pow(D, N)) + (B / pow(D, M));
			Helpers::normalise(R);
			R *= U;

			sum += R;
		}
	}
	applyForce(sum);
	update(1.f);
}