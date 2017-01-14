#include "Laser.h"
//TODO move textures out somewhere else (asset loader?)
Laser::Laser(const sf::Vector2f& startPos, const sf::Vector2f& direction)
	: GameObject(Type::Laser, startPos)
	, m_liveTimer(0.f)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;

	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * direction);
	m_dir = direction;
	m_velocity = m_dir * m_maxVelocity;
	m_moving = true;

	setOrigin();
}

void Laser::update(float dt)
{
	if (m_active)
	{
		m_liveTimer += dt;
		if (m_liveTimer > TTL)
		{
			m_active = false;
		}
	}
	GameObject::update(dt);
}
