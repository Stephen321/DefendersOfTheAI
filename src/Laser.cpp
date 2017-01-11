#include "Laser.h"
//TODO move textures out somewhere else (asset loader?)
Laser::Laser(const sf::Vector2f& startPos, const sf::Texture& texture, float startSpeed, const sf::Vector2f& direction)
	: GameObject(Type::Laser, startPos, texture, PhysicsProperties(800.f, 0.f, startSpeed + 800.f))
{
	m_dir = direction;
	m_velocity = m_dir * m_physicsProperties.MAX_VEL;
	m_force.x = 800.f;
}
