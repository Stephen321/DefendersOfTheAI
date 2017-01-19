#include "Meteor.h"

Meteor::Meteor(const sf::Vector2f& worldSize, const int radius)
	: GameObject(Type::Meteor, sf::Vector2f(), worldSize),
	m_radius(radius)
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;

	m_dir = sf::Vector2f(Helpers::randomNumberF(-0.5f, 0.5f), Helpers::randomNumberF(0.5f, 1.f));
	Helpers::normalise(m_dir);

	m_velocity = m_dir * m_maxVelocity;
	m_moving = true;

	setOrigin();

	const int screenUnit = worldSize.x / 9 / 128;
	int max_radius = 15 * screenUnit;


	
	m_colors[0] = sf::Color(222, 122, 145);
	m_colors[1] = sf::Color(196, 55, 98);
	m_colors[2] = sf::Color(119, 24, 52);
	m_colors[3] = sf::Color(86, 15, 36);
	m_colors[4] = sf::Color(36, 4, 13);

	generateShape(worldSize, screenUnit);
	m_position = m_shapes[0].getPosition();
}

void Meteor::generateShape(sf::Vector2f worldSize, int screenUnit)
{
	int xPos = worldSize.x - 200;// rand() % (int)(worldSize.x);
	createConvexCircle(screenUnit, m_radius, xPos, sf::Color(225, 155, 176));	//Rim Highlight
	createConvexCircle(screenUnit, m_radius - (screenUnit * 0.5f), xPos, sf::Color(196, 55, 98));							//Body		
	createConvexCircle(screenUnit, m_radius * 0.75f, xPos, sf::Color(0, 0, 0, 150));					//Outer Shadow
	createConvexCircle(screenUnit, m_radius * 0.5f, xPos, sf::Color().Black);						//Inner Shadow
}

void Meteor::createConvexCircle(int screenUnit, int radius, int xPos, sf::Color col)
{
	std::vector<sf::Vector2f> circlePath;
	//calculate the paths for each circle
	for (int i = 0; i < METEOR_FACES; i++)
	{
		float x = m_centre.x + radius * cos(2 * M_PI * i / METEOR_FACES);
		float y = m_centre.y + radius * sin(2 * M_PI * i / METEOR_FACES);
		circlePath.push_back(sf::Vector2f(x, y));
	}

	//plot the points along the path generated for the circle
	for (int i = 0; i < METEOR_FACES; i++)
	{
		sf::ConvexShape convex;
		convex.setPointCount(3);
		convex.setOrigin(m_centre);

		for (int j = 0; j < convex.getPointCount() - 1; j++)
		{
			if (i + j == circlePath.size())	//last point in the circle, must join up with first point	
			{
				convex.setPoint(j, circlePath[0]);
				break;
			}
			convex.setPoint(j, circlePath[i + j]);			
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
	m_velocity = m_dir * dt * 150.f;
	float angularVelocity = dt * 100;
	for (int i = 0; i < m_shapes.size(); i++)
	{		
		if (m_shapes[0].getPosition().y < m_worldSize.y - m_radius)
		{
			m_shapes[i].move(m_velocity);
			m_shapes[i].rotate(angularVelocity);	

			if (i >= METEOR_FACES && i < METEOR_FACES + METEOR_FACES)//the indices where the body of meteor was inserted
			{				
				float r = m_shapes[i].getRotation() + ((i - METEOR_FACES) * ANGLE_BETWEEN_POINTS);
				if (r > 360)
					r -= 360;

				if ((r > ANGLE_RANGES[0].first || r < ANGLE_RANGES[0].second))
					m_shapes[i].setFillColor(m_colors[4]);
				else if (r > ANGLE_RANGES[1].first && r < ANGLE_RANGES[1].second || r > ANGLE_RANGES[7].first && r < ANGLE_RANGES[7].second)
					m_shapes[i].setFillColor(m_colors[3]);
				else if (r > ANGLE_RANGES[2].first && r < ANGLE_RANGES[2].second || r > ANGLE_RANGES[6].first && r < ANGLE_RANGES[6].second)
					m_shapes[i].setFillColor(m_colors[2]);
				else if (r > ANGLE_RANGES[3].first && r < ANGLE_RANGES[3].second || r > ANGLE_RANGES[5].first && r < ANGLE_RANGES[5].second)
					m_shapes[i].setFillColor(m_colors[1]);
				else if (r > ANGLE_RANGES[4].first && r < ANGLE_RANGES[4].second)
					m_shapes[i].setFillColor(m_colors[0]);			
			}
		}
		else 
		{//test
			//m_active = false;
		}
		checkWorldBounds();
		m_position = m_shapes[0].getPosition();
	}	
}

void Meteor::lerpFaceColor(float currentRotation, const std::pair<float, float> angleRange, sf::Color startColor, sf::Color endColor)
{

}

void Meteor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_shapes.size(); i++)
	{
		target.draw(m_shapes[i]);	
	}
}

sf::FloatRect Meteor::getRect() const
{
	const int pointCount = m_shapes[0].getPointCount();
	int minX = INT_MAX;
	int minY = INT_MAX;
	int maxX = 0;
	int maxY = 0;
	int diameter = m_radius * 2;

	for (int i = 0; i < m_shapes.size() * 0.25f; i++)
	{
		if (m_shapes[i].getPosition().x - m_radius < minX)
			minX = m_shapes[i].getPosition().x - m_radius;

		if (m_shapes[i].getPosition().y - m_radius < minY)
			minY = m_shapes[i].getPosition().y - m_radius;
	}

	return sf::FloatRect(minX, minY, diameter, diameter);
}

void Meteor::checkWorldBounds()
{
	if (m_position.x < -m_radius)
	{
		m_position.x = m_worldSize.x - m_radius;
		for (int i = 0; i < m_shapes.size(); i++)
		{			
			m_shapes[i].move(sf::Vector2f(m_worldSize.x, 0));
		}
	}
	else if (m_position.x > m_worldSize.x + m_radius)
	{
		m_position.x = m_radius;
		for (int i = 0; i < m_shapes.size(); i++)
		{
			m_shapes[i].move(sf::Vector2f());
		}
	}
}

float Meteor::getHeight() const
{
	return m_radius - 30.f;
}
