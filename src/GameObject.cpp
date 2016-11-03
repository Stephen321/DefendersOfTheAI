#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& startPos, const sf::Texture& texture)
	: m_position(startPos)
	, m_sprite(texture)
{
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}

void GameObject::update(float dt)
{
	move(dt);
	m_sprite.setPosition(m_position);
}

sf::Vector2f GameObject::getPosition() const
{
	return m_position;
}

sf::Vector2f GameObject::getVelocity() const
{
	return m_velocity;
}

void GameObject::move(float dt)
{
	m_acceleration = m_speed * m_dir * dt;//m_speed * m_dir;
	//m_velocity += m_acceleration;
	//m_position += m_velocity * dt;
	m_velocity = sf::Vector2f(250.f, 0.f)  * dt;
	m_position += m_velocity;
}
