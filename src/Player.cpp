#include "Player.h"

Player::Player(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& gameProjectiles)
	: GameObject(Type::Player, startPos, worldSize)
	, m_reloadTimer(0.f)
	, m_gameProjectiles(gameProjectiles)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	props.texture.setSmooth(true);
	m_sprite.setTexture(props.texture);
	//m_sprite.setScale(sf::Vector2f(0.5,0.5));
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;
	m_dir.x = 1.f;

	setOrigin();
}

void Player::update(float dt)
{
	checkInput();
	if (m_reloadTimer < RELOAD_TIME)
	{
		m_reloadTimer += dt;
	}
	m_sprite.setRotation(atan2(0, m_dir.x) * (180.f / M_PI) + ANGLE_OFFSET);
	GameObject::update(dt);
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
	m_gameProjectiles.push_back(std::shared_ptr<Laser>(new Laser(m_position + ((m_sprite.getGlobalBounds().width * 0.5f) * dir), m_worldSize, dir)));
}

void Player::checkInput()
{
	m_moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_dir.x = -1.f;
		m_sprite.setScale(1.f, 1.f);
		m_moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_dir.x = 1.f;
		m_sprite.setScale(-1.f, 1.f);
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
	Helpers::normalise(m_dir);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		fire();
	}
}
