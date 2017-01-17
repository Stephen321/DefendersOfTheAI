#include "Meteor.h"

Meteor::Meteor(const sf::Vector2f& startPos, const sf::Vector2f& direction)
	: GameObject(Type::Meteor, startPos, sf::Vector2f())
{
	GameData::ObjectProperties& props = GameData::getInstance().getObjectProperties((int)m_type);
	m_sprite.setTexture(props.texture);
	m_forceAmount = props.forceAmount;
	m_dragCoefficent = props.dragCoefficent;
	m_maxVelocity = props.maxVelocity;

	m_dir = direction;
	m_position += ((m_sprite.getGlobalBounds().width * 0.5f) * m_dir);
	m_velocity = m_dir * m_maxVelocity;
	m_moving = true;

	setOrigin();

	generateShape();
}

void Meteor::generateShape()
{
	sf::ConvexShape convex;	
	m_centre = sf::Vector2f(0, 300);
	int radius = 100;	

	for (int i = 0; i < METEOR_SEGMENTS; i++)
	{
		float x = m_centre.x + radius * cos(2 * PI * i / METEOR_SEGMENTS);
		float y = m_centre.y + radius * sin(2 * PI * i / METEOR_SEGMENTS);
		circle.push_back(sf::Vector2f(x, y));
	}	

	for (int i = 0; i < METEOR_SEGMENTS; i++)
	{
		convex.setPointCount(3);
		convex.setOrigin(m_centre);
		convex.setPosition(sf::Vector2f(100, 400));

		for (int j = 0; j < convex.getPointCount() - 1; j++)
		{		
			//if (j == 0 || j == convex.getPointCount() - 2)
			convex.setPoint(j, circle[i + j]);	

			if (i + j == circle.size())	//last point in the circle, must join up with first point		
				convex.setPoint(j, circle[0]);						
		}

		convex.setPoint((convex.getPointCount() - 1), m_centre);

		convex.setFillColor(sf::Color(rand() % 255,rand() % 255, rand() % 255));
		
		m_shapes.push_back(convex);		
	}	

	
}

void Meteor::update(float dt)
{	
	for (int i = 0; i < m_shapes.size(); i++)
	{
		m_shapes[i].rotate(dt * 100);
	}	

	GameObject::update(dt);
}

void Meteor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_shapes.size(); i++)
	{
		//if (i != 7)
			target.draw(m_shapes[i]);
	}
}