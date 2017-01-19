#include "Nest.h"


Nest::Nest(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const std::shared_ptr<GameObject> player,
		   GameObjectPtrVector& gameProjectiles, GameObjectPtrVector& gameAbductors)
	: AI(Type::Nest, startPos, worldSize)
	, m_gameProjectiles(gameProjectiles)
	, m_gameAbductors(gameAbductors)
	, m_targetPos(m_position)
	, m_wanderOrientation(atan2(m_dir.y, m_dir.x))
	, m_player(player)
	, m_missilesAlive(0)
	, m_abductorsProduced(0)
	, OFFSET_Y(worldSize.y * 0.3f)
	, m_timeToProduceAbductor(TIME_TO_PRODUCE + Helpers::randomNumber(-PRODUCE_TIME_OFFSET, PRODUCE_TIME_OFFSET))
{
	m_fsm.init(this);
	m_fsm.changeState(NWanderState::getInstance());
}

void Nest::setTargetPos(const sf::Vector2f & target)
{
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	m_targetPos = target;
}

bool Nest::checkIfReachedTarget()
{
	bool reachedTarget = false;
	if (Helpers::getLength(m_targetPos - m_position) <= MIN_TARGET_DIST)
	{
		reachedTarget = true;
	}
	return reachedTarget;
}

void Nest::getWanderTarget()
{
	float change = (rand() % 2 == 0) ? ANGLE_CHANGE : -ANGLE_CHANGE;
	change = change * (M_PI / 180.f);
	m_wanderOrientation += change;

	float currentOrientation = atan2(m_dir.y, m_dir.x);

	float targetOrientation = m_wanderOrientation + currentOrientation;

	m_targetPos = m_position + (WANDER_OFFSET * m_dir);

	sf::Vector2f targetDir;
	targetDir.x = cos(targetOrientation);
	targetDir.y = sin(targetOrientation);

	m_targetPos += WANDER_RADIUS * targetDir;


	if (m_targetPos.y <= m_sprite.getGlobalBounds().height * 0.5f)
	{
		m_targetPos += 1.5f * (((m_targetPos.y < 0) ? abs(m_targetPos.y) : 0) + m_sprite.getGlobalBounds().height * 0.5f) * sf::Vector2f(0.f, 1.f);
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
	}
	else if (m_targetPos.y > (LOWEST_DISTANCE- OFFSET_Y) - m_sprite.getGlobalBounds().height * 0.5f)
	{
		m_targetPos += 1.5f * (m_targetPos.y - (LOWEST_DISTANCE - OFFSET_Y ) + m_sprite.getGlobalBounds().height * 0.5f) * sf::Vector2f(0.f, -1.f);
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
	}
	m_dir = Helpers::normaliseCopy(m_targetPos - m_position);
	if (m_targetPos.x < 0.f)
	{
		m_dir = Helpers::normaliseCopy(m_targetPos - m_position);
		m_targetPos.x = m_worldSize.x - abs(m_targetPos.x);
	}
	else if (m_targetPos.x > m_worldSize.x)
	{
		m_dir = Helpers::normaliseCopy(m_targetPos - m_position);
		m_targetPos.x = m_targetPos.x - m_worldSize.x;
	}
}

bool Nest::playerInRange() const
{
	bool inRange = false;

	if (Helpers::getLength(Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_player->getPosition())) <= PLAYER_EVADE_RANGE)
	{
		inRange = true;
	}
	return inRange;
}


void Nest::fire(float dt)
{
	m_reloadTimer += dt;
	if (m_reloadTimer < RELOAD_TIME)
		return;
	if ((Helpers::getLength(m_player->getPosition() - m_position)) < PLAYER_MISSILE_RANGE
		&& m_missilesAlive < MAX_MISSILES_ALIVE)
	{
		m_reloadTimer = 0.f;
		m_missilesAlive++;
		sf::Vector2f startPos(m_position.x, m_position.y + (m_sprite.getGlobalBounds().height * 0.5f));
		m_gameProjectiles.push_back(std::shared_ptr<Missile>(new Missile(startPos, m_worldSize, m_playerPos, m_missilesAlive)));
	}
}

void Nest::setPlayerPos()
{
	m_playerPos = m_player->getPosition();
}

void Nest::setProduceTimer(float value)
{
	m_produceAbductorTimer = value;
}

void Nest::evade()
{
	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_player->getPosition());
	sf::Vector2f dir = Helpers::normaliseCopy(vectorBetween);
	float distance = Helpers::getLength(vectorBetween);
	float speed = Helpers::getLength(m_velocity);
	float prediction;
	if (speed <= distance / MAX_PREDICTON)
	{
		prediction = MAX_PREDICTON;
	}
	else
	{
		prediction = distance / speed;
	}

	m_targetPos = m_player->getPosition() + (m_player->getVelocity() * prediction);
	m_dir = Helpers::normaliseCopy(Helpers::getVectorBetweenWrap(m_worldSize, m_targetPos, m_position));
}

void Nest::produceAbductors(float dt)
{
	if (m_abductorsProduced < MAX_ABDUCTORS_PRODUCED)
	{
		m_produceAbductorTimer += dt;
		float offset = m_sprite.getGlobalBounds().height * 1.5f;
		if ((LOWEST_DISTANCE - OFFSET_Y ) - m_position.y <= m_sprite.getGlobalBounds().height * 2.f) //is there isnt room to spawn an abductor below
		{
			offset = -offset; //then spawn above
		}
		if (m_produceAbductorTimer > m_timeToProduceAbductor)
		{
			//new abductor
			sf::Vector2f startPos(m_position.x, m_position.y + offset);
			m_gameAbductors.push_back(std::shared_ptr<Abductor>(new Abductor(startPos, m_worldSize, m_gameAbductors, m_player, m_gameProjectiles)));
			m_gameAbductors.back()->setVelocity(sf::Vector2f(m_velocity.x, 0.f));
			//update direction to move in the same x and a random y dir
			m_gameAbductors.back()->setDirection(sf::Vector2f((m_velocity.x > 0.f) ? 1.f : -1.f, 1.f));
			m_abductorsProduced++;
			m_produceAbductorTimer = 0.f;
			m_timeToProduceAbductor = TIME_TO_PRODUCE + Helpers::randomNumber(-PRODUCE_TIME_OFFSET, PRODUCE_TIME_OFFSET);
		}
	}
}


void Nest::checkWorldBounds()
{
	GameObject::checkWorldBounds();
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y > (LOWEST_DISTANCE - OFFSET_Y) - halfHeight)
	{
		m_position.y = (LOWEST_DISTANCE - OFFSET_Y) - halfHeight;
		m_velocity.y = 0.f;
	}
}