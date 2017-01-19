#include "Astronaut.h"


Astronaut::Astronaut(float startX, const sf::Vector2f& worldSize, const std::vector<sf::Vector2i>& surfacePathPoints)
	: AI(Type::Astronaut, sf::Vector2f(), worldSize)
	, m_surfacePathPoints(surfacePathPoints)
	, m_angle(0.f)
	, m_beingAbducted(false)
{
	m_fsm.init(this);
	m_fsm.changeState(0);//TODO: put astronaut states in here

	m_dir = sf::Vector2f((rand() % 2 == 0) ? -1.f : 1.f, 0);
	m_moving = true;
	m_position = sf::Vector2f(startX, getYAtX(startX));
}

void Astronaut::update(float dt)
{
	if (m_beingAbducted == false)
	{
		m_sprite.setRotation(m_angle);
		AI::update(dt);
		m_position.y = getYAtX(m_position.x);
	}
	//else abductor is controlling us
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
}

bool Astronaut::getBeingAbductd() const
{
	return m_beingAbducted;
}

float Astronaut::getYAtX(float x)
{
	if (x < m_surfacePathPoints.back().x && x >= 0.f )
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
		float y = (pointBefore + (sf::Vector2i)(Helpers::normaliseCopy((sf::Vector2f)vectorBetween) * (x - pointBefore.x))).y;
		y -= m_sprite.getGlobalBounds().height * 0.5f + SURFACE_RIM_THICKNESS;
		return y;
	}
	return 0.f;
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
