#include "Laser.h"
//TODO move textures out somewhere else (asset loader?)
Laser::Laser(const sf::Vector2f& startPos, const sf::Texture& texture, const sf::Vector2f& direction)
	: GameObject(Type::Laser, startPos, texture, PhysicsProperties(800.f, 0.f, 1600.f))
	, m_liveTimer(0.f)
{
	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * direction);
	m_dir = direction;
	m_velocity = m_dir * m_physicsProperties.MAX_VEL;
	m_moving = true;
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
