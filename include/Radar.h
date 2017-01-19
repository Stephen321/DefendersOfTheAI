#pragma once
#include "GameObject.h"
#include <vector>

class Radar : public sf::Drawable
{
public:
	Radar(std::vector<sf::Vector2i> terrainPath);//, const std::vector<GameObject>* gameObjects);
	~Radar();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(int playerDirection, sf::Vector2f& playerPos, const sf::FloatRect& cameraBounds);
	void createLines(std::vector<sf::Vector2i> terrainPath);
private:
	std::vector<sf::ConvexShape> m_terrainLinesShapes;
	sf::ConvexShape playerIcon;
	std::vector<float> m_lineWidths;
	sf::FloatRect m_bounds;
};