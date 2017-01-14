#include "Player.h"

Player::Player(const sf::Vector2f& startPos, const sf::Texture& texture, const sf::Texture& tempLaserTex)
	: GameObject(Type::Player, startPos, texture, PhysicsProperties(800.f, 0.9f, 1000.f))
	, m_tempLaserTex(tempLaserTex)
	, m_reloadTimer(0.f)
{
	m_dir.x = 1.f;
}

void Player::update(float dt)
{
	checkInput();
	if (m_position.y - (m_sprite.getGlobalBounds().height * 0.5f) < 0) //TODO: override GameObject::checkWorldBounds()
	{
		m_position.y = m_sprite.getGlobalBounds().height * 0.5f;
		m_velocity.y = 0;
	}
	if (m_reloadTimer < RELOAD_TIME)
	{
		m_reloadTimer += dt;
	}
	for (Laser& l : m_lasers)
	{
		l.update(dt);
	}
	GameObject::update(dt);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GameObject::draw(target, states);
	for (const Laser& l : m_lasers)
	{
		target.draw(l);
	}
}

void Player::teleport(float offset, int section, float width)
{
	for (Laser& l : m_lasers)
	{
		l.teleport(offset, section, width);
	}
	GameObject::teleport(offset, section, width);
}

void Player::fire()
{
	if (m_reloadTimer < RELOAD_TIME)
		return;

	m_reloadTimer = 0.f;
	sf::Vector2f dir = m_dir;
	if (dir.x < 0.f)
	{
		dir.x = -1.f;
	}
	else if (dir.x > 0.f)
	{
		dir.x = 1.f;
	}
	dir.y = 0.f;
	m_lasers.push_back(Laser(m_position + ((m_sprite.getGlobalBounds().width * 0.5f) * dir), m_tempLaserTex, dir));
}

void Player::checkInput()
{
	m_moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_dir.x = -1.f;
		m_moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_dir.x = 1.f;
		m_moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_dir.y = -1.f;
		m_moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_dir.y = 1.f;
		m_moving = true;
	}
	m_dir = Helpers::normalise(m_dir);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		fire();
	}
}
