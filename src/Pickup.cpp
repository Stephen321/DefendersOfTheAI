#include "Pickup.h"

Pickup::Pickup(GameObject::Type type, const sf::Vector2f& worldSize)
	: GameObject(type, sf::Vector2f(), worldSize)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;

	m_dir = sf::Vector2f(0, 1);
	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * m_dir);
	m_velocity = m_dir * m_maxVelocity * m_fallingSpeed;
	m_moving = true;
	setOrigin();

	m_position.x = rand() % (int)m_worldSize.x;
	m_position.y -= m_sprite.getGlobalBounds().height;
}

void Pickup::update(float dt)
{
	if (m_position.y + m_sprite.getGlobalBounds().height >= m_worldSize.y)
		m_active = false;

	GameObject::update(dt);
}