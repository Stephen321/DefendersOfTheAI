#include "GameObject.h"

GameObject::GameObject(Type type, const sf::Vector2f& startPos, const sf::Texture& texture, const PhysicsProperties& physicProperties)
	: m_type(type)
	, m_position(sf::Vector2f(startPos.x, startPos.y))
	, m_sprite(texture)
	, m_physicsProperties(physicProperties)
	, m_dir()
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

void GameObject::moveBy(float dx, float dy)
{
	m_position.x += dx;
	m_position.y += dy;
	m_sprite.setPosition(m_position);
}

void GameObject::teleport(float offset, int section, float width)
{
	int location = (int)m_position.x / width; //forced integer division to get number between 0 and max sections
	if (m_position.x < 0.f)
	{
		location--;
	}
	if (location == section)
	{
		setPosition(sf::Vector2f(m_position.x + offset, m_position.y));
	}
}

GameObject::Type GameObject::getType() const
{
	return m_type;
}

void GameObject::move(float dt)
{
	sf::Vector2f linearDrag;
	m_force = m_physicsProperties.MAX_FORCE * m_dir;

	if (Helpers::getLength(m_velocity) > 0)
	{// if moving and not holding key then apply linear drag on that axis
		if (m_force.x == 0.f)
		{
			linearDrag.x = m_physicsProperties.DRAG_COEFFICIENT * -m_velocity.x;
		}
		if (m_force.y == 0.f)
		{
			linearDrag.y = m_physicsProperties.DRAG_COEFFICIENT * -m_velocity.y;
		}
	}

	m_acceleration = m_force + linearDrag; //a = F/m
	m_velocity += m_acceleration * dt; //v = u + at

	if (Helpers::getLength(m_velocity) >= m_physicsProperties.MAX_VEL)
	{ 
		m_velocity = Helpers::normalise(m_velocity) * m_physicsProperties.MAX_VEL;
	}

	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2
}