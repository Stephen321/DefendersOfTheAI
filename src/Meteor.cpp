#include "Meteor.h"

Meteor::Meteor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
	: GameObject(Type::Meteor, startPos, sf::Vector2f())
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;
	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * m_dir);
	m_velocity = m_dir * m_maxVelocity;
	m_moving = true;

	setOrigin();

	SCREEN_HEIGHT = worldSize.y;

	sf::Vector2f randDir = sf::Vector2f(rand() % 5 + 5, rand() % 6 + 2);

	if (rand() % 2 == 0)
		randDir.x *= -1;

	m_dir = randDir;
	int screenUnit = worldSize.x / 9 / 128;
	m_radius = (rand() % 10 + 5) * screenUnit;
	generateShape(worldSize, screenUnit);
}

void Meteor::generateShape(sf::Vector2f worldSize, int screenUnit)
{		
	int xPos = rand() % (int)(worldSize.x);	
	createConvexCircle(screenUnit, m_radius + (screenUnit * 0.5f), xPos, sf::Color(225, 155, 176));	//Rim Highlight
	createConvexCircle(screenUnit, m_radius, xPos, sf::Color(196, 55, 98));								//Body		
	createConvexCircle(screenUnit, m_radius * 0.85f, xPos,	sf::Color(0,0,0,150));						//Outer Shadow
	createConvexCircle(screenUnit, m_radius * 0.65f, xPos, sf::Color().Black);							//Inner Shadow
}

void Meteor::createConvexCircle(int screenUnit, int radius, int xPos, sf::Color col)
{
	std::vector<sf::Vector2f> circlePath;
	//calculate the paths for each circle
	for (int i = 0; i < METEOR_SEGMENTS; i++)
	{
		float x = m_centre.x + radius * cos(2 * PI * i / METEOR_SEGMENTS);
		float y = m_centre.y + radius * sin(2 * PI * i / METEOR_SEGMENTS);
		circlePath.push_back(sf::Vector2f(x, y));
	}

	//plot the points along the path generated for the circle
	for (int i = 0; i < METEOR_SEGMENTS; i++)
	{
		sf::ConvexShape convex;
		convex.setPointCount(3);
		convex.setOrigin(m_centre);

		for (int j = 0; j < convex.getPointCount() - 1; j++)
		{
			convex.setPoint(j, circlePath[i + j]);

			if (i + j == circlePath.size())	//last point in the circle, must join up with first point		
				convex.setPoint(j, circlePath[0]);
		}
		convex.setPoint((convex.getPointCount() - 1), m_centre);

		convex.setFillColor(col);
		convex.setPosition(sf::Vector2f(xPos, -m_radius));
		m_shapes.push_back(convex);
	}
}

void Meteor::update(float dt)
{	
	for (int i = 0; i < m_shapes.size(); i++)
	{		
		if (m_shapes[0].getPosition().y < SCREEN_HEIGHT - SCREEN_HEIGHT / 16)
		{
			m_shapes[i].move(sf::Vector2f(m_dir));
			m_shapes[i].rotate(dt * 100);
		}
		else
			m_alive = false;
	}	

	GameObject::update(dt);
}

void Meteor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_shapes.size(); i++)
	{
		if (m_alive)
		{
			target.draw(m_shapes[i]);
		}
	}
}

bool Meteor::getAlive() const
{
	return m_alive;
}