#include "Radar.h"

Radar::Radar(std::vector<sf::Vector2i> terrainPath)//, const std::vector<GameObject>* gameObjects)
{
	createLines(terrainPath);
	playerIcon.setPointCount(3);
	playerIcon.setFillColor(sf::Color(255, 255, 255, 100));
	playerIcon.setOutlineThickness(1);
	playerIcon.setOutlineColor(sf::Color(255, 255, 255, 200));
	playerIcon.setPoint(0, sf::Vector2f(0, 5));
	playerIcon.setPoint(1, sf::Vector2f(20, 0));
	playerIcon.setPoint(2, sf::Vector2f(20, 10));
	playerIcon.setOrigin((playerIcon.getPoint(1).x - playerIcon.getPoint(0).x) * 0.5f, (playerIcon.getPoint(2).y - playerIcon.getPoint(1).y) * 0.5f);
}

Radar::~Radar()
{

}

void Radar::createLines(std::vector<sf::Vector2i> terrainPath)
{
	int lineThickness = 3;
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setFillColor(sf::Color(255, 255, 255, 100));
	float lineWidths = 0;
	float yPos = 0;

	for (int i = 0; i < terrainPath.size() - 1; i+=2)
	{
		convex.setPoint(0, sf::Vector2f(terrainPath[i].x * 0.25f, terrainPath[i].y * 0.25f));
		convex.setPoint(1, sf::Vector2f(terrainPath[i + 1].x * 0.25f, terrainPath[i + 1].y * 0.25f));
		convex.setPoint(2, sf::Vector2f(terrainPath[i + 1].x * 0.25f, terrainPath[i + 1].y * 0.25f + lineThickness));
		convex.setPoint(3, sf::Vector2f(terrainPath[i].x * 0.25f, terrainPath[i].y * 0.25f + lineThickness));

		m_terrainLinesShapes.push_back(convex);
		m_lineWidths.push_back(terrainPath[i + 1].x * 0.25f - terrainPath[i].x * 0.25f);
	}	
}

void Radar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	float lineWidths = 0;
	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		if (m_terrainLinesShapes[i].getPosition().x + lineWidths > m_bounds.left &&
			m_terrainLinesShapes[i].getPosition().x + lineWidths < m_bounds.left + m_bounds.width)
		{
			target.draw(m_terrainLinesShapes[i]);
		}		
		lineWidths += m_lineWidths[i];
	}

	target.draw(playerIcon);
}

void Radar::update(int playerDirection, sf::Vector2f& playerPos, const sf::FloatRect& cameraBounds)
{
	m_bounds = sf::FloatRect(cameraBounds.left + 400, cameraBounds.top, cameraBounds.width - 800, cameraBounds.height);
	for (int i = 0; i < m_terrainLinesShapes.size(); i++)
	{
		m_terrainLinesShapes[i].move(sf::Vector2f(playerPos.x * 0.75f - m_terrainLinesShapes[i].getPosition().x, 0));
	}	

	if (playerDirection != 0 && playerDirection != playerIcon.getScale().x)
		playerIcon.scale(sf::Vector2f(playerDirection, 1.f));

	playerIcon.move(sf::Vector2f(playerPos.x - playerIcon.getPosition().x, playerPos.y * 0.25f - playerIcon.getPosition().y));
}