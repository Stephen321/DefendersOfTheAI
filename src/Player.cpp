#include "Player.h"

Player::Player(const sf::Vector2f& startPos, const sf::Texture& texture, const sf::Texture& tempLaserTex)
	: GameObject(startPos, texture, PhysicsProperties(800.f, 0.9f, 1000.f))
	, m_tempLaserTex(tempLaserTex)
{
}

void Player::update(float dt)
{
	checkInput();
	if (m_position.y - (m_sprite.getGlobalBounds().height * 0.5f) < 0) //TODO: override GameObject::checkWorldBounds()
	{
		m_velocity.y = 0;
		m_force.y = 0;
	}
	GameObject::move(dt);
	m_sprite.setPosition(m_position);

	for (Laser& l : m_lasers)
	{
		l.update(dt);
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GameObject::draw(target, states);
	for (const Laser& l : m_lasers)
	{
		target.draw(l);
	}
}

void Player::fire()
{
	m_lasers.push_back(Laser(m_position, m_tempLaserTex, m_velocity));
}

void Player::checkInput()
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
	m_dir = Helpers::normalise(dir);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		fire();
	}
}
