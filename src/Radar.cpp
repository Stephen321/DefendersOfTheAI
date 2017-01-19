#include "Radar.h"

Radar::Radar(std::vector<sf::Vector2i> terrainPath, const sf::Vector2f& worldSize)
{
	createLines(terrainPath, worldSize);

	//setup entity shapes
	m_playerShape = createEntityShape(sf::Color(0, 255, 0, 120), sf::Color(0, 255, 255, 200), 6, 6);
	m_enemyShape = createEntityShape(sf::Color(255, 0, 0, 120), sf::Color(255, 0, 0, 200), 6, 6);
}

Radar::~Radar()
{

}

sf::ConvexShape Radar::createEntityShape(const sf::Color bodyColor, const sf::Color outlineColor, const int width, const int height)
{
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setFillColor(bodyColor);
	convex.setOutlineThickness(1);
	convex.setOutlineColor(outlineColor);

	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(width, 0));
	convex.setPoint(2, sf::Vector2f(width, height));
	convex.setPoint(3, sf::Vector2f(0, height));

	//set the origin to the middle of the shape
	convex.setOrigin((convex.getPoint(1).x - convex.getPoint(0).x) * 0.5f,
					 (convex.getPoint(2).y - convex.getPoint(1).y) * 0.5f);
	return convex;
}

void Radar::createLines(std::vector<sf::Vector2i> terrainPath, const sf::Vector2f& worldSize)
{
	const int lineThickness = 3;
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setFillColor(sf::Color(255, 255, 255, 100));

	//create each shape in the surface radar representation using the path generated in surface
	for (int i = 0; i < terrainPath.size() - 1; i+=2)
	{
		convex.setPoint(0, sf::Vector2f(terrainPath[i].x	 *	RADAR_SCALAR, 
										terrainPath[i].y	 *  RADAR_SCALAR));

		convex.setPoint(1, sf::Vector2f(terrainPath[i + 1].x *	RADAR_SCALAR, 
										terrainPath[i + 1].y *  RADAR_SCALAR));

		convex.setPoint(2, sf::Vector2f(terrainPath[i + 1].x *	RADAR_SCALAR,
										terrainPath[i + 1].y *  RADAR_SCALAR + lineThickness));

		convex.setPoint(3, sf::Vector2f(terrainPath[i].x	 *	RADAR_SCALAR, 
										terrainPath[i].y	 *  RADAR_SCALAR + lineThickness));

		m_terrainLinesShapes.push_back(convex);
		m_lineWidths.push_back(terrainPath[i + 1].x * RADAR_SCALAR - terrainPath[i].x * RADAR_SCALAR);
	}	

	//add shapes onto the end of the surface to accomodate for the gap created by its smaller size
	int width = 0;
	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		convex.setPoint(0, sf::Vector2f(terrainPath[i].x	  * RADAR_SCALAR + (worldSize.x * RADAR_SCALAR),
										terrainPath[i].y	  * RADAR_SCALAR));

		convex.setPoint(1, sf::Vector2f(terrainPath[i + 1].x  * RADAR_SCALAR + (worldSize.x * RADAR_SCALAR),
										terrainPath[i + 1].y  * RADAR_SCALAR));

		convex.setPoint(2, sf::Vector2f(terrainPath[i + 1].x  * RADAR_SCALAR + (worldSize.x * RADAR_SCALAR),
										terrainPath[i + 1].y  * RADAR_SCALAR + lineThickness));

		convex.setPoint(3, sf::Vector2f(terrainPath[i].x	  * RADAR_SCALAR + (worldSize.x * RADAR_SCALAR),
										terrainPath[i].y	  * RADAR_SCALAR + lineThickness));

		m_terrainLinesShapes.push_back(convex);
		m_lineWidths.push_back(m_lineWidths[i]);
		width += m_lineWidths[i];
		if (width > (worldSize.x / Constants::WORLD_SCREEN_SIZES)  * RADAR_SCALAR)
			break;
	}
}

void Radar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i].first == GameObject::Type::Abductor ||
			m_gameObjects[i].first == GameObject::Type::Nest)
		{
			sf::ConvexShape enemy = m_enemyShape;
			enemy.move(sf::Vector2f(m_gameObjects[i].second.x - enemy.getPosition().x,
									m_gameObjects[i].second.y * RADAR_SCALAR - enemy.getPosition().y));
			target.draw(enemy);
		}
		else if (m_gameObjects[i].first == GameObject::Type::Astronaut)
		{
			sf::ConvexShape astronaut = m_astronautShape;
			astronaut.move(sf::Vector2f(m_gameObjects[i].second.x - astronaut.getPosition().x,
				m_gameObjects[i].second.y * RADAR_SCALAR - astronaut.getPosition().y));
			target.draw(astronaut);
		}
	}

	float lineWidths = 0;

	//render all the shapes in the radar that are within the custom bounds
	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		if (m_terrainLinesShapes[i].getPosition().x + lineWidths > m_bounds.left &&
			m_terrainLinesShapes[i].getPosition().x + lineWidths < m_bounds.left + m_bounds.width)
		{
			target.draw(m_terrainLinesShapes[i]);
		}		

		lineWidths += m_lineWidths[i];
	}

	target.draw(m_playerShape);
}

void Radar::update(sf::Vector2f& playerPos, const sf::FloatRect& cameraBounds, std::vector<std::pair<GameObject::Type, sf::Vector2f>>& gameObjects)
{
	m_gameObjects = gameObjects;

	//create new bounds smaller than the camera so the radar occupies a smaller area of screen
	m_bounds = sf::FloatRect(cameraBounds.left + BOUNDS_BUFFER, 
							 cameraBounds.top, 
							 cameraBounds.width - BOUNDS_BUFFER * 2, 
							 cameraBounds.height);

	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		m_terrainLinesShapes[i].move(sf::Vector2f(playerPos.x * (1 - RADAR_SCALAR) - m_terrainLinesShapes[i].getPosition().x, 0));
	}	

	m_playerShape.move(sf::Vector2f(playerPos.x - m_playerShape.getPosition().x, 
								 playerPos.y * RADAR_SCALAR - m_playerShape.getPosition().y));
}