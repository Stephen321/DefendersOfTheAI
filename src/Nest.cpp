#include "Nest.h"


Nest::Nest(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, const std::shared_ptr<GameObject> player)
	: AI(Type::Nest, startPos, worldSize)
	, m_targetPos(m_position)
	, LOWEST_DISTANCE(worldSize.y * 0.65f)
	, m_wanderOrientation(atan2(m_dir.y, m_dir.x))
	, m_player(player)
{
	m_fsm.init(this);
	m_fsm.changeState(NWanderState::getInstance());


	testCircle.setOutlineColor(sf::Color::Green);
	testCircle.setFillColor(sf::Color::Transparent);
	testCircle.setOutlineThickness(2.f);
	testCircle.setPosition(m_targetPos);
	testCircle.setRadius(WANDER_RADIUS);
	testCircle.setOrigin(testCircle.getRadius(), testCircle.getRadius());

	testCircle2.setFillColor(sf::Color(0, 0, 0, 255));
	testCircle2.setPosition(m_targetPos);
	testCircle2.setRadius(5.f);
	testCircle2.setOrigin(testCircle2.getRadius(), testCircle2.getRadius());

	testCircle3.setFillColor(sf::Color(0, 120, 0, 128));
	testCircle3.setPosition(m_targetPos);
	testCircle3.setRadius(MIN_TARGET_DIST);
	testCircle3.setOrigin(testCircle3.getRadius(), testCircle3.getRadius());


	testCircle4.setFillColor(sf::Color(255, 255, 255, 128));
	testCircle4.setPosition(m_position);
	testCircle4.setRadius(5.f);
	testCircle4.setOrigin(testCircle4.getRadius(), testCircle4.getRadius());

	testCircle5.setFillColor(sf::Color(200, 200, 200, 50));
	testCircle5.setPosition(m_position);
	testCircle5.setRadius(PLAYER_IN_RANGE);
	testCircle5.setOrigin(testCircle5.getRadius(), testCircle5.getRadius());
}

void Nest::setTargetPos(const sf::Vector2f & target)
{
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	m_targetPos = target;


	testCircle.setPosition(m_targetPos);
}

void Nest::update(float dt)
{
	testCircle4.setPosition(m_position);
	testCircle5.setPosition(m_position);

	m_reloadTimer += dt;
	for (Missile& m : m_missiles)
	{
		m.update(dt);
	}
	AI::update(dt);
}

bool Nest::checkIfReachedTarget()
{
	bool reachedTarget = false;
	if (Helpers::getLength(m_targetPos - m_position) <= MIN_TARGET_DIST)
	{
		reachedTarget = true;
	}
	return reachedTarget;
}

void Nest::getWanderTarget(float offsetScale)
{
	float change = (rand() % 2 == 0) ? ANGLE_CHANGE : -ANGLE_CHANGE;
	change = change * (M_PI / 180.f);
	m_wanderOrientation += change;

	float currentOrientation = atan2(m_dir.y, m_dir.x);
	//std::cout << "currentOrientation: " << currentOrientation << std::endl;

	float targetOrientation = m_wanderOrientation + currentOrientation;

	m_targetPos = m_position + (offsetScale * WANDER_OFFSET * m_dir);
	testCircle.setPosition(m_targetPos);

	sf::Vector2f targetDir;
	targetDir.x = cos(targetOrientation);
	targetDir.y = sin(targetOrientation);
	//std::cout << "targetOrientation: " << targetOrientation << std::endl;

	m_targetPos += WANDER_RADIUS * targetDir;
	testCircle2.setPosition(m_targetPos);
	testCircle3.setPosition(m_targetPos);
	m_dir = Helpers::normaliseCopy(m_targetPos - m_position); // direction might have changed if target did


	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_targetPos.y < halfHeight)
	{
		m_dir.y = -m_dir.y;
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
		getWanderTarget(0.5f);
	}
	else if (m_targetPos.y > LOWEST_DISTANCE - halfHeight)
	{
		m_dir.y = -m_dir.y;
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
		getWanderTarget(0.5f);
	}
	if (m_targetPos.x < -(m_worldSize.x / Constants::WORLD_SCREEN_SIZES) - halfWidth)
	{
		m_dir.x = -m_dir.x;
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
		getWanderTarget(0.5f);
	}
	else if (m_targetPos.x > m_worldSize.x + (m_worldSize.x / Constants::WORLD_SCREEN_SIZES) + halfWidth)
	{
		m_dir.x = -m_dir.x;
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
		getWanderTarget(0.5f);
	}


}

bool Nest::playerInRange() const
{
	bool inRange = false;
	if (Helpers::getLength(m_player->getPosition() - m_position) <= PLAYER_IN_RANGE)
	{
		inRange = true;
	}
	return inRange;
}


void Nest::fire()
{
	if (m_reloadTimer < RELOAD_TIME)
		return;
	m_reloadTimer = 0.f;
	m_missiles.push_back(Missile(sf::Vector2f(m_position.x, m_position.y + (m_sprite.getGlobalBounds().height * 0.5f)), m_player->getPosition() + m_player->getVelocity()));
}


void Nest::evade()
{
	sf::Vector2f vectorBetween = m_player->getPosition() - m_position;
	sf::Vector2f dir = Helpers::normaliseCopy(vectorBetween);
	float distance = Helpers::getLength(vectorBetween);
	float speed = Helpers::getLength(m_velocity);
	float prediction;
	if (speed <= distance / MAX_PREDICTON)
	{
		prediction = MAX_PREDICTON;
	}
	else
	{
		prediction = distance / speed;
	}

	m_targetPos = m_player->getPosition() + (m_player->getVelocity() * prediction);
	m_dir = Helpers::normaliseCopy(m_position - m_targetPos);

	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < halfHeight)
	{
		m_velocity.y = 0.f;
	}
	else if (m_position.y > LOWEST_DISTANCE - halfHeight)
	{
		m_velocity.y = 0.f;
	}
	if (m_position.x < halfWidth)
	{
		m_velocity.x = 0.f;
	}
	else if (m_position.x > m_worldSize.x - halfWidth)
	{
		m_velocity.x = 0.f;
	}

	testCircle2.setPosition(m_targetPos);
}

//TODO: way to have this in Character superclass
bool Nest::teleport(float offset, int section, float width)
{
	for (Missile& m : m_missiles)
	{
		m.teleport(offset, section, width);
	}
	bool teleported = GameObject::teleport(offset, section, width);
	if (teleported)
	{
		m_targetPos = (sf::Vector2f(m_targetPos.x + offset, m_targetPos.y));
	}
	
	return teleported;
}

void Nest::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const Missile& m : m_missiles)
	{
		m.draw(target, states);
	}
	GameObject::draw(target, states);
	target.draw(testCircle);
	target.draw(testCircle2);
	target.draw(testCircle3);
	target.draw(testCircle4);
	target.draw(testCircle5);
}
