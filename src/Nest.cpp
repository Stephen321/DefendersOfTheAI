#include "Nest.h"


Nest::Nest(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const std::shared_ptr<GameObject> player, std::vector<std::shared_ptr<GameObject>>& gameMissiles)
	: AI(Type::Nest, startPos, worldSize)
	, m_gameProjectiles(gameMissiles)
	, m_targetPos(m_position)
	, m_wanderOrientation(atan2(m_dir.y, m_dir.x))
	, m_player(player)
	, m_missilesAlive(0)
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
	else if (m_targetPos.y > LOWEST_DISTANCE - m_sprite.getGlobalBounds().height * 0.5f)
	{
		m_targetPos += 1.5f * (m_targetPos.y - LOWEST_DISTANCE + m_sprite.getGlobalBounds().height * 0.5f) * sf::Vector2f(0.f, -1.f);
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
	if (Helpers::getLength(m_player->getPosition() - m_position) <= PLAYER_EVADE_RANGE)
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

void Nest::evade()
{
	sf::Vector2f vectorBetween = m_player->getPosition() - m_position;
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
	m_dir = Helpers::normaliseCopy(m_position - m_targetPos);
}

void Nest::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	GameObject::draw(target, states);
}
