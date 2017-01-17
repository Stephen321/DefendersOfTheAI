#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Abductor.h"

Abductor::Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::vector<std::shared_ptr<Abductor>>& abductors)
	: AI(Type::Abductor, startPos, worldSize)
	, m_abductors(abductors)
	, LOWEST_DISTANCE(worldSize.y * 0.65f)
	, START_MAX_VEL(m_maxVelocity)
{
	m_velocity = sf::Vector2f(Helpers::randomNumberF(-2, 2), Helpers::randomNumberF(-2, 2)); // Allows for range of -2 -> 2
	m_fsm.init(this);
	m_fsm.changeState(APatrolState::getInstance());
}

// Function that checks and modifies the distance
// of a Abductor if it breaks the law of separation.
sf::Vector2f Abductor::separation()
{
	sf::Vector2f steer(0, 0);
	int count = 0;
	// For every Abductor in the system, check if it's too close
	for (int i = 0; i < m_abductors.size(); i++)
	{
		// Calculate distance from current Abductor to Abductor we're looking at
		float d = Helpers::getLength(m_position - m_abductors[i]->getPosition());
		// If this is a fellow Abductor and it's too close, move away from it
		if (d > 0 && d < DESIRED_SEPARATION)
		{
			sf::Vector2f diff = m_position - m_abductors[i]->getPosition();
			Helpers::normalise(diff);
			diff /= d;      // Weight by distance. Further away doesnt influence as much
			steer += diff;
			count++;
		}
	}
	// Adds average difference of m_position to m_acceleration
	if (count > 0)
		steer /= (float)count;
	if (Helpers::getLength(steer) > 0) 
	{
		// Steering = Desired - Velocity
		Helpers::normalise(steer);
		steer *= m_maxVelocity;
		steer -= m_velocity;
		Helpers::limit(steer, m_forceAmount);
	}
	return steer * SEPERATION_WEIGHT;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Abductor passed as parameter to adjust to that
// of nearby Abductors.
sf::Vector2f Abductor::alignment()
{
	sf::Vector2f sum(0, 0);	
	int count = 0;
	for (int i = 0; i < m_abductors.size(); i++)
	{
		float d = Helpers::getLength(m_position - m_abductors[i]->getPosition());
		if (d > 0 && d < NEIGHBOUR_RADIUS)
		{
			sum += m_abductors[i]->getVelocity();
			count++;
		}
	}
	// If there are Abductors close enough for alignment...
	if (count > 0)
	{
		sum /= (float)count;// Divide sum by the number of close Abductors (average of velocity)
		Helpers::normalise(sum);	   		// Turn sum into a unit vector, and
		sum *= m_maxVelocity;    // Multiply by maxSpeed
		// Steer = Desired - Velocity
		sf::Vector2f steer; 
		steer = sum - m_velocity; //sum = desired(average)  
		Helpers::limit(steer, m_forceAmount);
		return steer * ALIGNMENT_WEIGHT;
	} else {
		sf::Vector2f temp(0, 0);
		return temp;
	}
}

// Cohesion finds the average m_position of nearby Abductors and manipulates the 
// steering force to move in that direction.
sf::Vector2f Abductor::cohesion()
{
	sf::Vector2f sum(0, 0);	
	int count = 0;
	for (int i = 0; i < m_abductors.size(); i++)
	{
		float d = Helpers::getLength(m_position - m_abductors[i]->getPosition());
		if (d > 0 && d < NEIGHBOUR_RADIUS)
		{
			sum += m_abductors[i]->getPosition();
			count++;
		}
	}
	if (count > 0)
	{
		sum /= (float)count;
		return seek(sum) * COHESION_WEIGHT;
	} else {
		return sf::Vector2f();
	}
}

// Seek function limits the maxSpeed, finds necessary steering force and
// normalizes the vectors.
sf::Vector2f Abductor::seek(const sf::Vector2f& v)
{
	sf::Vector2f desired = v - m_position;  // A vector pointing from the m_position to the target
	// Normalize desired and scale to maximum speed
	Helpers::normalise(desired);
	desired *= m_maxVelocity;
	// Steering = Desired minus Velocity
	sf::Vector2f steer;
	steer = desired - m_velocity;
	Helpers::limit(steer, m_forceAmount);  // Limit to maximum steering force
	return steer;
}

//Update modifies velocity, m_position, and resets m_acceleration with values that
//are given by the three laws.
void Abductor::update(float dt)
{
	m_fsm.update(dt);
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(angle(m_velocity));
}

void Abductor::setAcceleration(const sf::Vector2f & acceleration)
{
	m_acceleration = acceleration;
}

void Abductor::checkBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < halfHeight || m_position.y > LOWEST_DISTANCE - halfHeight)
	{
		m_velocity.y = -m_velocity.y * 0.9f;
	}

	if (m_position.x < -(m_worldSize.x / Constants::WORLD_SCREEN_SIZES) - halfWidth)
	{
		m_position.x = m_worldSize.x + (m_worldSize.x / Constants::WORLD_SCREEN_SIZES) + halfWidth;
	}
	else if (m_position.x > m_worldSize.x + (m_worldSize.x / Constants::WORLD_SCREEN_SIZES) + halfWidth)
	{
		m_position.x = -(m_worldSize.x / Constants::WORLD_SCREEN_SIZES) - halfWidth;
	}
}

void Abductor::move(float dt)
{
	m_velocity += m_acceleration  * dt; //v = u + at
	Helpers::limit(m_velocity, m_maxVelocity);
	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2
}

int Abductor::getNeighbourCount() const
{
	int count = 0;
	for (int i = 0; i < m_abductors.size(); i++)
	{
		float d = Helpers::getLength(m_position - m_abductors[i]->getPosition());
		if (this != m_abductors[i].get() && d < NEIGHBOUR_RADIUS)
		{
			count++;
		}
	}
	return count;
}

void Abductor::setYPosWave()
{
	m_position.y = LOWEST_DISTANCE * 0.5f + (float)cos(m_position.x / (LOWEST_DISTANCE * 0.25f)) * (LOWEST_DISTANCE * 0.5f - m_sprite.getGlobalBounds().height);
	//TODO: change velocity .x to match
}

float Abductor::getForceAmount() const
{
	return m_forceAmount;
}

void Abductor::setDirection(const sf::Vector2f & dir)
{
	m_dir = dir;
}

sf::Vector2f Abductor::getDirection() const
{
	return m_dir;
}

void Abductor::resetMaxVelocity()
{
	m_maxVelocity = START_MAX_VEL;
}

void Abductor::setMaxPatrolVelocity()
{
	m_maxVelocity = START_MAX_VEL * PATROL_MAX_VEL_SCALE;
}

// Calculates the angle for the velocity of a Abductor which allows the visual 
// image to rotate in the direction that it is going in.
float Abductor::angle(const sf::Vector2f& v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180.f / M_PI);
	return angle;
}