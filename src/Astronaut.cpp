#include "Astronaut.h"


Astronaut::Astronaut(float startX, const sf::Vector2f& worldSize, const std::vector<sf::Vector2i>& surfacePathPoints)
	: AI(Type::Astronaut, sf::Vector2f(), worldSize)
	, m_surfacePathPoints(surfacePathPoints)
	, m_beingAbducted(false)
	, m_beingChased(false)
{
	m_dir = sf::Vector2f((rand() % 2 == 0) ? -1.f : 1.f, 0);
	m_moving = true;
	m_position = sf::Vector2f(startX, getYAtX(startX));
	m_healthBar.setYOffset(-1000.f);
	m_fsm.init(this);
	m_fsm.changeState(AsWanderState::getInstance());//TODO: put astronaut states in here
}

void Astronaut::checkWorldBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < halfHeight)
	{
		m_active = false;
	}
	if (m_position.x < 0.f)
	{
		m_position.x = m_worldSize.x;
	}
	else if (m_position.x > m_worldSize.x)
	{
		m_position.x = 0.f;
	}
}

void Astronaut::setBeingAbducted(bool value)
{
	m_beingAbducted = value;
	changeState(AsWanderState::getInstance());
}

void Astronaut::setBeingChased(bool value)
{
	m_beingChased = value;
}

bool Astronaut::getBeingAbducted() const
{
	return m_beingAbducted;
}

bool Astronaut::getBeingChased() const
{
	return m_beingChased;
}

void Astronaut::setTarget(const sf::Vector2f & target)
{
	m_targetPos = target;
}

float Astronaut::getYAtX(float x)
{
	if (x <= m_surfacePathPoints.back().x && x >= 0.f )
	{
		int index = Helpers::binarySearch(m_surfacePathPoints, sf::Vector2i(x, 0), 
		[](const sf::Vector2i& v1, const sf::Vector2i& v2) -> bool { //equals
			return v1.x == v2.x;
		},
		[](const sf::Vector2i& v1, const sf::Vector2i& v2) -> bool { //less than 
			return v1.x <= v2.x;
		});
		sf::Vector2i pointBefore = m_surfacePathPoints[index];
		sf::Vector2i pointAfter = pointBefore;
		if (pointBefore.x != x && index + 1 < m_surfacePathPoints.size())
			pointAfter = m_surfacePathPoints[index + 1];
		sf::Vector2i vectorBetween = pointAfter - pointBefore;
		float y = (float)(pointBefore + (sf::Vector2i)(Helpers::normaliseCopy((sf::Vector2f)vectorBetween) * (x - pointBefore.x))).y;
		y -= m_sprite.getGlobalBounds().height * 0.5f + SURFACE_RIM_THICKNESS;
		return y;
	}
	return 0.f;
}

void Astronaut::setAbductor(const std::shared_ptr<GameObject>& abductor)
{
	m_abductor = abductor;
}

const std::shared_ptr<GameObject>& Astronaut::getAbductor() const
{
	return m_abductor;
}

void Astronaut::seekAbductionPos()
{
	sf::Vector2f vectorBetween = Helpers::getVectorBetweenWrap(m_worldSize, m_position, m_targetPos);
	float distance = Helpers::getLength(vectorBetween);


	if (Helpers::getLength(m_targetPos - m_position) < TARGET_RANGE)
	{
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
	}

	float targetSpeed;

	if (distance > ARRIVE_RADIUS)
	{
		targetSpeed = m_maxVelocity;
	}
	else
	{
		targetSpeed = m_maxVelocity * distance / ARRIVE_RADIUS;
	}

	sf::Vector2f targetVelocity = Helpers::normaliseCopy(vectorBetween) * targetSpeed;

	m_acceleration = targetVelocity - m_velocity;
	m_acceleration /= TIME_TO_TARGET;
	Helpers::limit(m_acceleration, m_forceAmount);
}


void Astronaut::move(float dt)
{
	m_velocity += m_acceleration  * dt; //v = u + at
	Helpers::limit(m_velocity, m_maxVelocity);
	m_position += m_velocity * dt + (0.5f * (m_acceleration * (dt * dt))); // s = ut + 0.5at^2

	m_sprite.setPosition(m_position);
}

void Astronaut::updateAcceleration()
{
	m_acceleration = m_dir * m_forceAmount;
}

//void Astronaut::setAngleAtX()
//{
//	if (m_position.x < m_surfacePathPoints.back().x && m_position.x >= 0.f)
//	{
//		int index = Helpers::binarySearch(m_surfacePathPoints, sf::Vector2i(m_position.x, 0),
//			[](const sf::Vector2i& v1, const sf::Vector2i& v2) -> bool { //equals
//			return v1.x == v2.x;
//		},
//			[](const sf::Vector2i& v1, const sf::Vector2i& v2) -> bool { //less than 
//			return v1.x <= v2.x;
//		});
//		sf::Vector2i pointBefore = m_surfacePathPoints[index];
//		sf::Vector2i pointAfter = pointBefore;
//		if (pointBefore.x != m_position.x && index + 1 < m_surfacePathPoints.size())
//			pointAfter = m_surfacePathPoints[index + 1];
//		sf::Vector2f dir = Helpers::normaliseCopy((sf::Vector2f(pointAfter - pointBefore)));
//		m_angle = atan2(dir.y, dir.x);
//	}
//	else
//	{
//		m_angle = 0.f;
//	}
//}
