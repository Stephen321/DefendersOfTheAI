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
	m_healthBar = HealthBar(props.maxHealth);
	m_healthBar.setYOffset(-(m_sprite.getGlobalBounds().height * 0.5f + HEALTH_Y_OFFSET));
	m_healthBar.setXSize(m_sprite.getGlobalBounds().width);
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
	m_healthBar.update(m_position);
	m_sprite.setRotation(atan2(0, m_dir.x) * (180.f / M_PI) + ANGLE_OFFSET);
	GameObject::update(dt);
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_healthBar);
	GameObject::draw(target, states);
}

bool Player::collision(const std::shared_ptr<GameObject>& collidor)
{
	bool collided = false;
	if (GameObject::collision(collidor))
	{
		collided = true;

		float damage = 0.f;
		if (collidor->getType() == Type::Laser)
		{
			collidor->setActive(false);
			std::shared_ptr<Laser> laser = std::static_pointer_cast<Laser>(collidor);
			damage = laser->getDamage();
		}
		else if (collidor->getType() == Type::Missile)
		{
			collidor->setActive(false);
			std::shared_ptr<Missile> missile = std::static_pointer_cast<Missile>(collidor);
			damage = missile->getDamage();
		}

		if (m_healthBar.changeHealth(-damage) == false)
		{
			//TODO: kill player
		}
	}
	return collided;
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
	m_gameProjectiles.push_back(std::shared_ptr<Laser>(new Laser(m_position + ((m_sprite.getGlobalBounds().width * 0.5f) * dir), m_worldSize, dir, DAMAGE, m_type)));
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
	Helpers::normalise(m_dir);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		fire();
	}
}
