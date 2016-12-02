#include "GameObject.h"

GameObject::GameObject(const sf::Vector2u& worldSize, const sf::Vector2f& startPos, const sf::Texture& texture)
	: m_position(sf::Vector2f(startPos.x, startPos.y))
	, m_sprite(texture)
	, m_dir()
	, FORCE(800.f)
	, m_worldSize(worldSize)
	, DRAG_COEFFICIENT(0.9f)
	, MAX_VEL(1000.f)
{
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width * 0.5f, m_sprite.getLocalBounds().height * 0.5f);
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

void GameObject::setPosition(const sf::Vector2f & v)
{
	m_position = v;
	m_sprite.setPosition(m_position);
}

sf::Vector2f GameObject::getVelocity() const
{
	return m_velocity;
}

void GameObject::move(float dx, float dy)
{
	m_position.x += dx;
	m_position.y += dy;
	m_sprite.setPosition(m_position);
}

void GameObject::move(float dt)
{
	calcForce();
	sf::Vector2f linearDrag;

	if (Helpers::getLength(m_velocity) > 0)
	{// if moving and not holding key then apply linear drag on that axis
		if (m_force.x == 0.f)
		{
			linearDrag.x = DRAG_COEFFICIENT * -m_velocity.x;
		}
		if (m_force.y == 0.f)
		{
			linearDrag.y = DRAG_COEFFICIENT * -m_velocity.y;
		}
	}

	m_acceleration = m_force + linearDrag; //a = F/m
	m_velocity += m_acceleration * dt; //v = u + at

	if (Helpers::getLength(m_velocity) >= MAX_VEL)
	{ 
		m_velocity = Helpers::normalise(m_velocity) * MAX_VEL;
	}

	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2
	checkWorldBounds();
}

void GameObject::calcForce()
{
	m_force = FORCE * m_dir;
}

void GameObject::checkWorldBounds()
{
	//if (m_position.x > m_worldSize.x)
	//{
	//	m_position.x = 0;
	//}
	//else if (m_position.x < 0)
	//{
	//	m_position.x = m_worldSize.x;
	//}
}