#include "Meteor.h"

Meteor::Meteor(const sf::Vector2f& worldSize)
	: GameObject(Type::Meteor, sf::Vector2f(), sf::Vector2f())
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

	int screenUnit = worldSize.x / 9 / 128;
	m_radius = (rand() % 10 + 5) * screenUnit;
	int max_radius = 15 * screenUnit;

	sf::Vector2f randDir = sf::Vector2f(rand() % screenUnit * 0.25f + screenUnit * 0.1f , rand() % screenUnit * 0.25f + screenUnit * 0.1f * max_radius / m_radius);

	if (rand() % 2 == 0)
		randDir.x *= -1;

	m_dir = randDir;
	
	m_colors[0] = sf::Color(222, 122, 145);
	m_colors[1] = sf::Color(196, 55, 98);
	m_colors[2] = sf::Color(119, 24, 52);
	m_colors[3] = sf::Color(86, 15, 36);
	m_colors[4] = sf::Color(36, 4, 13);

	generateShape(worldSize, screenUnit);
}

void Meteor::generateShape(sf::Vector2f worldSize, int screenUnit)
{
	int xPos = rand() % (int)(worldSize.x);
	createConvexCircle(screenUnit, m_radius + (screenUnit * 0.5f), xPos, sf::Color(225, 155, 176));	//Rim Highlight
	createConvexCircle(screenUnit, m_radius, xPos, sf::Color(196, 55, 98));							//Body		
	createConvexCircle(screenUnit, m_radius * 0.75f, xPos, sf::Color(0, 0, 0, 150));					//Outer Shadow
	createConvexCircle(screenUnit, m_radius * 0.5f, xPos, sf::Color().Black);						//Inner Shadow
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
		convex.setPosition(sf::Vector2f(xPos, -m_radius));
		convex.setRotation(360.f - 22.5f);		
		convex.setFillColor(col);	
		
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
			
			if (i >= METEOR_SEGMENTS && i < METEOR_SEGMENTS * 2) //the indices where the body of meteor was inserted
			{
				float shapeRot = m_shapes[i].getRotation() + ((i - METEOR_SEGMENTS) * 45);
				if (shapeRot > 360)
					shapeRot -= 360;

				if ((shapeRot > 337.5f || shapeRot < 22.5f))
					m_shapes[i].setFillColor(m_colors[4]);
				else if ((shapeRot > 22.5f && shapeRot < 67.5f) || (shapeRot > 292.5f && shapeRot < 337.5f))
					m_shapes[i].setFillColor(m_colors[3]);
				else if ((shapeRot > 67.5f && shapeRot < 112.5) || (shapeRot > 247.5f && shapeRot < 292.5f))
					m_shapes[i].setFillColor(m_colors[2]);
				else if ((shapeRot > 112.5f && shapeRot < 157.5f) || (shapeRot > 202.5f && shapeRot < 247.5f))
					m_shapes[i].setFillColor(m_colors[1]);
				else if ((shapeRot > 157.5f && shapeRot < 202.5f))
					m_shapes[i].setFillColor(m_colors[0]);				
			}
		}
		else
			m_active = false;
	}	

	GameObject::update(dt);
}

void Meteor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_shapes.size(); i++)
	{		
		target.draw(m_shapes[i]);		
	}
}