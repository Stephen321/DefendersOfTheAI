#include "Player.h"

Player::Player(const sf::Vector2f& startPos, const sf::Texture& texture)
	: GameObject(startPos, texture)
{
}

void Player::update(float dt)
{
	if (m_position.y - m_sprite.getGlobalBounds().height / 2.f < 0)
	{
		m_velocity.y = 0;
		m_force.y = 0;
	}
	move(dt);
	m_sprite.setPosition(m_sprite.getPosition().x, m_position.y);
}

void Player::calcForce()
{
	sf::Vector2f dir;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		dir.x = -1.f;
		if (m_velocity.x > 0.f)
		{//instantly change direction
			m_velocity.x = 0;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		dir.x = 1.f;
		if (m_velocity.x < 0.f)
		{//instantly change direction
			m_velocity.x = 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		dir.y = -1.f; 
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		dir.y = 1.f;
	}
	m_dir = normalise(dir);
	m_force = m_dir * FORCE;
}
