#include "GameObject.h"

GameObject::GameObject(Type type, const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
	: m_worldSize(worldSize)
	, m_moving(false)
	, m_type(type)
	, m_position(sf::Vector2f(startPos.x, startPos.y))
	, m_forceAmount(0.f)
	, m_dragCoefficent(0.f)
	, m_maxVelocity(0.f)
	, m_dir()
	, m_active(true)
{
}

void GameObject::setOrigin()
{
	m_sprite.setOrigin(m_sprite.getLocalBounds().width * 0.5f, m_sprite.getLocalBounds().height * 0.5f);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}

void GameObject::update(float dt)
{
	if (m_active == false)
	{
		return;
	}
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

bool GameObject::getActive() const
{
	return m_active;
}

void GameObject::move(float dt)
{
	sf::Vector2f linearDrag;
	float force = (m_moving) ? m_forceAmount : 0.f;
	if (m_moving == false)
	{
		if (abs(m_velocity.x) > MIN_VEL)
		{
			linearDrag.x = m_dragCoefficent * -m_velocity.x;
		}
		else
		{
			m_velocity.x = 0.f;
		}
		if (abs(m_velocity.y) > MIN_VEL)
		{
			linearDrag.y = m_dragCoefficent * -m_velocity.y;
		}
		else
		{
			m_velocity.y = 0.f;
		}
	}

	m_acceleration = (force * m_dir) + linearDrag; //a = F/m
	m_velocity += m_acceleration * dt; //v = u + at

	if (Helpers::getLength(m_velocity) >= m_maxVelocity)
	{ 
		m_velocity = Helpers::normaliseCopy(m_velocity) * m_maxVelocity;
	}


	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2
}