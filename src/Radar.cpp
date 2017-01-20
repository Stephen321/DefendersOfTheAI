#include "Radar.h"

Radar::Radar(std::vector<sf::Vector2i> terrainPath, const sf::Vector2f& worldSize)
{
	createLines(terrainPath, worldSize);

	//setup entity shapes
	m_playerShape = createEntityShape(sf::Color(0, 255, 0, 120), sf::Color(0, 255, 255, 200));
	m_enemyShape = createEntityShape(sf::Color(255, 0, 0, 120), sf::Color(255, 0, 0, 200));
	m_astronautShape = createEntityShape(sf::Color(55, 0, 255, 120), sf::Color(55, 0, 255, 200));
	m_meteorShape = createEntityShape(sf::Color(255, 255, 0, 120), sf::Color(255, 255, 0, 200));
	m_radarXPos = 0.f;
}

Radar::~Radar()
{

}

sf::ConvexShape Radar::createEntityShape(const sf::Color bodyColor, const sf::Color outlineColor)
{
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setFillColor(bodyColor);
	convex.setOutlineThickness(1);
	convex.setOutlineColor(outlineColor);

	convex.setPoint(0, sf::Vector2f(0,			 0));
	convex.setPoint(1, sf::Vector2f(ENTITY_SIZE, 0));
	convex.setPoint(2, sf::Vector2f(ENTITY_SIZE, ENTITY_SIZE));
	convex.setPoint(3, sf::Vector2f(0,			 ENTITY_SIZE));

	//set the origin to the middle of the shape
	convex.setOrigin((convex.getPoint(1).x - convex.getPoint(0).x) * 0.5f,
					 (convex.getPoint(2).y - convex.getPoint(1).y) * 0.5f);
	return convex;
}

void Radar::createLines(const std::vector<sf::Vector2i>& terrainPath, const sf::Vector2f& worldSize)
{
	const int LINE_THICKNESS = 3;
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setFillColor(sf::Color(255, 255, 0, 100));	
	shapesCopiedToStart = 0;

	for (int i = terrainPath.size() - 1; i > 0; i -= 2)
	{
		m_leftSideWidth += terrainPath[i].x * X_SCALE - terrainPath[i - 1].x * X_SCALE;

		if (m_leftSideWidth > ((worldSize.x / Constants::WORLD_SCREEN_SIZES)  * X_SCALE) * 2.f)		
			break;
		
		shapesCopiedToStart++;
	}

	/*for (int i = terrainPath.size() - 1 - (shapesCopiedToStart); i < terrainPath.size() - 1; i += 2)
	{
		convex.setPoint(0, sf::Vector2f(terrainPath[i].x	  * X_SCALE - (worldSize.x * X_SCALE),
		terrainPath[i].y	  * Y_SCALE));

		convex.setPoint(1, sf::Vector2f(terrainPath[i + 1].x  * X_SCALE - (worldSize.x * X_SCALE),
			terrainPath[i + 1].y  * Y_SCALE));

		convex.setPoint(2, sf::Vector2f(terrainPath[i + 1].x  * X_SCALE - (worldSize.x * X_SCALE),
			terrainPath[i + 1].y  * Y_SCALE + LINE_THICKNESS));

		convex.setPoint(3, sf::Vector2f(terrainPath[i].x	  * X_SCALE - (worldSize.x * X_SCALE),
			terrainPath[i].y	  * Y_SCALE + LINE_THICKNESS));

		m_terrainLinesShapes.push_back(convex);
		m_lineWidths.push_back(terrainPath[i + 1].x * X_SCALE - terrainPath[i].x * X_SCALE);		
	}*/

	convex.setFillColor(sf::Color(255, 255, 255, 100));
	//create each shape in the surface radar representation using the path generated in surface
	for (int i = 0; i < terrainPath.size() - 1; i+=2)
	{
		convex.setPoint(0, sf::Vector2f(terrainPath[i].x	 *	X_SCALE, 
										terrainPath[i].y	 *  Y_SCALE));

		convex.setPoint(1, sf::Vector2f(terrainPath[i + 1].x *	X_SCALE, 
										terrainPath[i + 1].y *  Y_SCALE));

		convex.setPoint(2, sf::Vector2f(terrainPath[i + 1].x *	X_SCALE,
										terrainPath[i + 1].y *  Y_SCALE + LINE_THICKNESS));

		convex.setPoint(3, sf::Vector2f(terrainPath[i].x	 *	X_SCALE, 
										terrainPath[i].y	 *  Y_SCALE + LINE_THICKNESS));

		m_terrainLinesShapes.push_back(convex);
		m_lineWidths.push_back(terrainPath[i + 1].x * X_SCALE - terrainPath[i].x * X_SCALE);
	}	


	convex.setFillColor(sf::Color::Magenta);
	int width = 0;
	//add shapes onto the end of the surface to accomodate for the gap created by its smaller size
	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		convex.setPoint(0, sf::Vector2f(terrainPath[i].x	  * X_SCALE + (worldSize.x * X_SCALE),
										terrainPath[i].y	  * Y_SCALE));

		convex.setPoint(1, sf::Vector2f(terrainPath[i + 1].x  * X_SCALE + (worldSize.x * X_SCALE),
										terrainPath[i + 1].y  * Y_SCALE));

		convex.setPoint(2, sf::Vector2f(terrainPath[i + 1].x  * X_SCALE + (worldSize.x * X_SCALE),
										terrainPath[i + 1].y  * Y_SCALE + LINE_THICKNESS));

		convex.setPoint(3, sf::Vector2f(terrainPath[i].x	  * X_SCALE + (worldSize.x * X_SCALE),
										terrainPath[i].y	  * Y_SCALE + LINE_THICKNESS));

		m_terrainLinesShapes.push_back(convex);
		m_lineWidths.push_back(m_lineWidths[i]);
		width += m_lineWidths[i];
		if (width > (worldSize.x / Constants::WORLD_SCREEN_SIZES)  * X_SCALE)
			break;
	}

	for (int i = 0; i < m_terrainLinesShapes.size() - 1; i++)
		std::cout << m_terrainLinesShapes[i].getPosition().x << std::endl;
}

void Radar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i].first == GameObject::Type::Abductor ||
			m_gameObjects[i].first == GameObject::Type::Nest)
		{
			sf::ConvexShape enemy = m_enemyShape;
			enemy.move(sf::Vector2f(m_gameObjects[i].second.x * X_SCALE - enemy.getPosition().x + m_radarXPos,
									m_gameObjects[i].second.y * (Y_SCALE)-enemy.getPosition().y));

			if (enemy.getPosition().x > m_bounds.left && enemy.getPosition().x  < m_bounds.left + m_bounds.width)
			{
				target.draw(enemy);
			}
		}
		else if (m_gameObjects[i].first == GameObject::Type::Astronaut)
		{
			sf::ConvexShape astronaut = m_astronautShape;
			astronaut.move(sf::Vector2f(m_gameObjects[i].second.x * X_SCALE - astronaut.getPosition().x + m_radarXPos,
										m_gameObjects[i].second.y * Y_SCALE - astronaut.getPosition().y));

			if (astronaut.getPosition().x > m_bounds.left && astronaut.getPosition().x  < m_bounds.left + m_bounds.width)
			{
				target.draw(astronaut);
			}
		}
		else if (m_gameObjects[i].first == GameObject::Type::Meteor)
		{
			sf::ConvexShape meteor = m_meteorShape;
			meteor.move(sf::Vector2f(m_gameObjects[i].second.x * X_SCALE - meteor.getPosition().x + m_radarXPos,
				m_gameObjects[i].second.y * Y_SCALE - meteor.getPosition().y));

			if (meteor.getPosition().x > m_bounds.left && meteor.getPosition().x  < m_bounds.left + m_bounds.width)
			{
				target.draw(meteor);
			}
		}
	}

	float lineWidths = 0;
	//render all the shapes in the radar that are within the custom bounds
	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		if (m_terrainLinesShapes[i].getPosition().x + lineWidths + 50.f > m_bounds.left &&
			m_terrainLinesShapes[i].getPosition().x + lineWidths - 50.f < m_bounds.left + m_bounds.width)
		{
			target.draw(m_terrainLinesShapes[i]);
		}		

		lineWidths += m_lineWidths[i];
	}

	target.draw(m_playerShape);
}

void Radar::update(const sf::Vector2f& playerPos, const sf::FloatRect& cameraBounds, const std::vector<std::pair<GameObject::Type, sf::Vector2f>>& gameObjects)
{
	m_gameObjects = gameObjects;
	m_radarXPos = playerPos.x - playerPos.x * X_SCALE;

	//create new bounds smaller than the camera so the radar occupies a smaller area of screen
	m_bounds = sf::FloatRect(cameraBounds.left + BOUNDS_BUFFER, 
							 cameraBounds.top, 
							 cameraBounds.width - BOUNDS_BUFFER * 2, 
							 cameraBounds.height);

	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		m_terrainLinesShapes[i].setPosition(sf::Vector2f(m_radarXPos, 0));
	}	

	m_playerShape.setPosition(sf::Vector2f(playerPos.x,
										 playerPos.y * Y_SCALE));
}