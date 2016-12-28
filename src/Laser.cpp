#include "Laser.h"
//TODO move textures out somewhere else (asset loader?)
Laser::Laser(const sf::Vector2f& startPos, const sf::Texture& texture, const sf::Vector2f& startVelocity)
	: GameObject(startPos, texture, PhysicsProperties(Helpers::getLength(startVelocity) + 800.f, 0.9f, 1000.f))
{
	m_dir = Helpers::normalise(startVelocity);
}
