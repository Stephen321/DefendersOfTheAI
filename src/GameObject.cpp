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
	checkWorldBounds();
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

void GameObject::setVelocity(const sf::Vector2f & v)
{
	m_velocity = v;
}

void GameObject::moveBy(float dx, float dy)
{
	m_position.x += dx;
	m_position.y += dy;
	m_sprite.setPosition(m_position);
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
	sf::Vector2f acceleration;
	if (m_moving == false)
	{
		acceleration = calculateLinearDrag();

		if (abs(m_velocity.x) <= MIN_VEL)
		{
			m_velocity.x = 0.f;
		}
		if (abs(m_velocity.y) <= MIN_VEL)
		{
			m_velocity.y = 0.f;
		}
	}
	acceleration += (force * m_dir); //a = F/m
	m_velocity += acceleration * dt; //v = u + at
	Helpers::limit(m_velocity, m_maxVelocity);

	m_position += m_velocity * dt + (0.5f * (acceleration * (dt * dt))); // s = ut + 0.5at^2
}

sf::Vector2f GameObject::calculateLinearDrag()
{
	sf::Vector2f linearDrag;
	if (abs(m_velocity.x) > MIN_VEL)
	{
		linearDrag.x = m_dragCoefficent * -m_velocity.x;
	}
	if (abs(m_velocity.y) > MIN_VEL)
	{
		linearDrag.y = m_dragCoefficent * -m_velocity.y;
	}
	return linearDrag;
}

void GameObject::checkWorldBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < halfHeight)
	{
		m_position.y = halfHeight;
		m_velocity.y = 0.f;
	}		
	else if (m_position.y > m_worldSize.y - halfHeight)
	{
		m_position.y = m_worldSize.y - halfHeight;
		m_velocity.y = 0.f;
	}
	if (m_position.x < -halfWidth)
	{
		m_position.x = m_worldSize.x - halfWidth;
	}
	else if (m_position.x > m_worldSize.x + halfWidth)
	{
		m_position.x = halfWidth;
	}
}

float GameObject::getWidth() const
{
	return m_sprite.getGlobalBounds().width;
}
