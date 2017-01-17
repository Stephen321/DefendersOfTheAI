#include "Missile.h"

Missile::Missile(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const sf::Vector2f& target)
	: GameObject(Type::Missile, startPos, worldSize)
	, m_liveTimer(0.f)
	, m_finishedDropping(false)
	, m_startY(startPos.y)
	, m_target(target)
	, m_angle(0.f)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;

	m_dir = sf::Vector2f(0, 1);
	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * m_dir);
	m_velocity = m_dir * m_maxVelocity * 0.25f;
	m_moving = true;

	setOrigin();
}

void Missile::update(float dt)
{
	if (m_active)
	{
		if (m_finishedDropping)
		{
			m_liveTimer += dt;
			if (m_liveTimer > TTL)
			{
				m_active = false;
			}
		}
		else
		{
			if (m_position.y - m_startY > DROP_DIST)
			{
				m_dir.y = 0.f;
				m_moving = false;
				if (m_velocity.y < 50.f)
				{
					m_finishedDropping = true;
					m_moving = true;
					m_dir = Helpers::normaliseCopy(m_target - m_position);
					m_velocity.y = 0.f;
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