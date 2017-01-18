#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Abductor.h"

Abductor::Abductor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameAbductors, 
				   std::shared_ptr<GameObject> player, GameObjectPtrVector& gameProjectiles)
	: AI(Type::Abductor, startPos, worldSize)
	, m_gameAbductors(gameAbductors)
	, m_gameProjectiles(gameProjectiles)
	, m_player(player)
	, HIGHEST_DISTANCE(worldSize.y * 0.35f)
	, m_reachedTarget(false)
{
	m_fsm.init(this);
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	m_targetInZone = Helpers::randomNumberF(HIGHEST_DISTANCE + halfHeight, LOWEST_DISTANCE - halfHeight); // where in the target zone we should go
	if (m_targetInZone < m_position.y) //if target is above the player then no need to move up to reach it
	{
		m_targetInZone = m_position.y;
	}
	int neighbourCount = getNeighbourCount();
	if (neighbourCount == 0)
	{
		m_fsm.changeState(ADropState::getInstance());
	}
	else
	{
		m_fsm.changeState(ADropState::getInstance());
	}
}

void Abductor::fire(float dt)
{
	m_reloadTimer += dt;
	if (m_reloadTimer < RELOAD_TIME)
		return;
	sf::Vector2f vectorBetween = m_player->getPosition() - m_position;
	if (Helpers::getLength(vectorBetween) < PLAYER_LASER_RANGE)
	{
		m_reloadTimer = 0.f;
		m_gameProjectiles.push_back(std::shared_ptr<Laser>(new Laser(m_position, m_worldSize, Helpers::normaliseCopy(vectorBetween), LASER_VEL_SCALE)));
	}
}

// Function that checks and modifies the distance
// of a Abductor if it breaks the law of separation.
sf::Vector2f Abductor::separation()
{
	sf::Vector2f steer(0, 0);
	int count = 0;
	// For every Abductor in the system, check if it's too close
	for (int i = 0; i < m_gameAbductors.size(); i++)
	{
		// Calculate distance from current Abductor to Abductor we're looking at
		float d = Helpers::getLength(m_position - m_gameAbductors[i]->getPosition());
		// If this is a fellow Abductor and it's too close, move away from it
		if (d > 0 && d < DESIRED_SEPARATION)
		{
			sf::Vector2f diff = m_position - m_gameAbductors[i]->getPosition();
			Helpers::normalise(diff);
			diff /= d;      // Weight by distance. Further away doesnt influence as much
			steer += diff;
			count++;
		}
	}
	//check if player is nearby
	float d = Helpers::getLength(m_position - m_player->getPosition());
	// If this is a fellow Abductor and it's too close, move away from it
	if (d < PLAYER_DESIRED_SEPARATION)
	{
		sf::Vector2f diff = m_position - m_player->getPosition();
		Helpers::normalise(diff);
		diff /= d * PLAYER_SEPERATION_FORCE_SCALE;      
		steer += diff;
		count++;
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
	for (int i = 0; i < m_gameAbductors.size(); i++)
	{
		float d = Helpers::getLength(m_position - m_gameAbductors[i]->getPosition());
		if (d > 0 && d < NEIGHBOUR_RADIUS)
		{
			sum += m_gameAbductors[i]->getVelocity();
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
	for (int i = 0; i < m_gameAbductors.size(); i++)
	{
		float d = Helpers::getLength(m_position - m_gameAbductors[i]->getPosition());
		if (d > 0 && d < NEIGHBOUR_RADIUS)
		{
			sum += m_gameAbductors[i]->getPosition();
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

void Abductor::setAcceleration(const sf::Vector2f & acceleration)
{
	m_acceleration = acceleration;
}

void Abductor::checkWorldBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < HIGHEST_DISTANCE + halfHeight)
	{
		m_position.y = HIGHEST_DISTANCE + halfHeight;
		m_velocity.y = -m_velocity.y * 0.9f;
	}
	else if (m_position.y > LOWEST_DISTANCE - halfHeight)
	{
		m_position.y = LOWEST_DISTANCE - halfHeight;
		m_velocity.y = -m_velocity.y * 0.9f;
	}
	if (m_position.x < -halfWidth)
	{
		m_position.x = m_worldSize.x - halfWidth;
	}
	else if (m_position.x > m_worldSize.x + halfWidth)
	{
		m_position.x = halfWidth;
	}
}

void Abductor::move(float dt)
{
	m_velocity += m_acceleration  * dt; //v = u + at
	Helpers::limit(m_velocity, m_maxVelocity);
	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2

	m_sprite.setPosition(m_position);
}

int Abductor::getNeighbourCount() const
{
	int count = 0;
	for (int i = 0; i < m_gameAbductors.size(); i++)
	{
		float d = Helpers::getLength(m_position - m_gameAbductors[i]->getPosition());
		if (this != m_gameAbductors[i].get() && d < NEIGHBOUR_RADIUS)
		{
			count++;
		}
	}
	return count;
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

void Abductor::updateDropAcceleration()
{
	//add on the patrol accel
	sf::Vector2f vectorBetween = sf::Vector2f(m_position.x, m_targetInZone) - m_position;
	float distance = Helpers::getLength(vectorBetween);


	if (abs(m_targetInZone - m_position.y) < TARGET_RANGE)
	{
		m_velocity.y = 0.f;
		m_reachedTarget = true;
		return;
	}

	float targetSpeed;

	if (distance > ARRIVE_RADIUS)
	{
		targetSpeed = m_maxVelocity;
	}
	else
	{
		targetSpeed = m_maxVelocity * distance / ARRIVE_RADIUS;
	}
	
	sf::Vector2f targetVelocity = Helpers::normaliseCopy(vectorBetween) * targetSpeed;
	m_acceleration = targetVelocity - m_velocity;
	m_acceleration /= TIME_TO_TARGET;
	Helpers::limit(m_acceleration, m_forceAmount);
}

void Abductor::updatePatrolAcceleration()
{
	m_acceleration = getForceAmount() * m_dir;
	m_acceleration += separation();
}

bool Abductor::reachedTarget() const
{
	return m_reachedTarget;
}