#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Mutant.h"

Mutant::Mutant(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, std::shared_ptr<GameObject> player, GameObjectMap& gameObjectsRef)
	: AI(Type::Mutant, startPos, worldSize)
	, m_gameObjectsRef(gameObjectsRef)
	, m_player(player)

{
	m_fsm.init(this);
	m_fsm.changeState(MSwarmState::getInstance());
	m_healthBar.setYOffset(-(m_sprite.getGlobalBounds().height * 0.5f + HEALTH_Y_OFFSET));
	m_healthBar.setXSize(m_sprite.getGlobalBounds().width);
	m_healthBar.setYSize(m_sprite.getGlobalBounds().height * 0.2f);
}

void Mutant::fire(float dt)
{
	m_reloadTimer += dt;
	if (m_reloadTimer < RELOAD_TIME)
		return;
	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_player->getPosition());
	if (Helpers::getLength(vectorBetween) < PLAYER_LASER_RANGE)
	{
		m_reloadTimer = 0.f;
		m_gameObjectsRef.at(Constants::PROJECTILE_KEY).push_back(std::shared_ptr<Laser>(new Laser(m_position, m_worldSize, Helpers::normaliseCopy(vectorBetween), DAMAGE, m_type, LASER_VEL_SCALE)));
	}
}

sf::Vector2f Mutant::separation()
{
	sf::Vector2f steer(0, 0);
	int count = 0;
	// For every Abductor in the system, check if it's too close
	for (int i = 0; i < m_gameObjectsRef.at(Constants::MUTANT_KEY).size(); i++)
	{
		// Calculate distance from current Mutant to Mutant we're looking at
		float d = Helpers::getLength(m_position - m_gameObjectsRef.at(Constants::MUTANT_KEY)[i]->getPosition());
		// If this is a fellow Mutant and it's too close, move away from it
		if (d > 0 && d < DESIRED_SEPARATION)
		{
			sf::Vector2f diff = m_position - m_gameObjectsRef.at(Constants::MUTANT_KEY)[i]->getPosition();
			Helpers::normalise(diff);
			diff /= d;      // Weight by distance. Further away doesnt influence as much
			steer += diff;
			count++;
		}
	}
	float d = Helpers::getLength(Helpers::getVectorBetweenWrap(m_worldSize, m_player->getPosition(), m_position));
	if (d < PLAYER_DESIRED_SEPARATION)
	{
		sf::Vector2f diff = Helpers::getVectorBetweenWrap(m_worldSize, m_player->getPosition(), m_position);
		Helpers::normalise(diff);
		diff /= d * PLAYER_SEPERATION_FORCE_SCALE;
		steer += diff * PLAYER_FORCE_SCALER;
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

// Function that checks and modifies the distance
// of a Mutant if it breaks the law of separation.
sf::Vector2f Mutant::swarm()
{
	sf::Vector2f sum;
	int count = 0;
	for (int i = 0; i < m_gameObjectsRef.at(Constants::MUTANT_KEY).size(); i++)
	{
		if (this != m_gameObjectsRef.at(Constants::MUTANT_KEY)[i].get())
		{
			float A = 5000.f;
			float N = 2.f;
			float B = 1000.f;
			float M = 0.5f;

			sf::Vector2f	R;
			R = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_gameObjectsRef.at(Constants::MUTANT_KEY)[i]->getPosition());
			float D = Helpers::getLength(R);
			if (D != 0.f)
			{
				float U = (-A / pow(D, N)) + (B / pow(D, M));
				Helpers::normalise(R);
				R *= U;
				sum += R;
				count++;
			}
		}
	}
	if (count > 0)
	{
		sum /= (float)count;
	}
	return sum * SWARM_WEIGHT;
}

sf::Vector2f Mutant::seek()
{
	sf::Vector2f steer;

	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_player->getPosition());
	sf::Vector2f dir = Helpers::normaliseCopy(vectorBetween);
	dir *= Helpers::getLength(vectorBetween) * PLAYER_SEEK_FORCE_SCALE;
	steer += dir;
	return steer * SEEK_WEIGHT;
}

void Mutant::checkWorldBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < halfHeight)
	{
		m_position.y = halfHeight;
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

void Mutant::setAcceleration(const sf::Vector2f & acceleration)
{
	m_acceleration = acceleration;
}

void Mutant::move(float dt)
{
	m_velocity += m_acceleration  * dt; //v = u + at
	Helpers::limit(m_velocity, m_maxVelocity);
	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2
	
	m_sprite.setPosition(m_position);
}

int Mutant::getNeighbourCount() const
{
	int count = 0;
	for (int i = 0; i < m_gameObjectsRef.at(Constants::MUTANT_KEY).size(); i++)
	{
		float d = Helpers::getLength(m_position - m_gameObjectsRef.at(Constants::MUTANT_KEY)[i]->getPosition());
		if (this != m_gameObjectsRef.at(Constants::MUTANT_KEY)[i].get() && d < NEIGHBOUR_RADIUS)
		{
			count++;
		}
	}
	return count;
}

float Mutant::getForceAmount() const
{
	return m_forceAmount;
}

void Mutant::setDirection(const sf::Vector2f & dir)
{
	m_dir = dir;
}

sf::Vector2f Mutant::getDirection() const
{
	return m_dir;
}