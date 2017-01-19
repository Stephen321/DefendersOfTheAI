#include "Missile.h"

Missile::Missile(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, sf::Vector2f& target, int& ownerMissileCount)
	: GameObject(Type::Missile, startPos, worldSize)
	, m_liveTimer(0.f)
	, m_finishedDropping(false)
	, m_startY(startPos.y)
	, m_target(target)
	, m_angle(0.f)
	, m_ownerMissileCount(ownerMissileCount)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;

	m_dir = sf::Vector2f(0, 1);
	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * m_dir);
	m_velocity = m_dir * m_maxVelocity * VEL_DROP_STOP_SCALE;
	m_moving = true;
	setOrigin();
}

void Missile::update(float dt)
{
	if (m_active)
	{
		if (m_finishedDropping)
		{
			m_dir = Helpers::normaliseCopy(Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_target));

			m_liveTimer += dt;
			if (m_liveTimer > TTL)
			{
				m_active = false;
				m_ownerMissileCount--;
			}
		}
		else
		{
			if (m_position.y - m_startY > DROP_DIST)
			{
				m_dir.y = 0.f;
				m_moving = false;
				if (m_velocity.y < m_maxVelocity * (1.f - VEL_DROP_STOP_SCALE))
				{
					m_finishedDropping = true;
					m_moving = true;
					m_dir = Helpers::normaliseCopy(m_target - m_position);
				}
			}
		}
	}
	m_angle = (m_dir != sf::Vector2f()) ? atan2(m_dir.y, m_dir.x) : m_angle;
	m_sprite.setRotation(m_angle * (180.f / M_PI));
	GameObject::update(dt);
}

void Missile::checkWorldBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	if (m_position.x < -halfWidth)
	{
		m_position.x = m_worldSize.x - halfWidth;
	}
	else if (m_position.x > m_worldSize.x + halfWidth)
	{
		m_position.x = halfWidth;
	}
}