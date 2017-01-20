#pragma once
#include "GameObject.h"
#include "Constants.h"
#include <vector>

class Radar : public sf::Drawable
{
public:
	Radar(std::vector<sf::Vector2i> terrainPath, const sf::Vector2f& worldSize);
	~Radar();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(const sf::Vector2f& playerPos, const sf::FloatRect& cameraBounds, const std::vector<std::pair<GameObject::Type, sf::Vector2f>>& gameObjects);
	void createLines(const std::vector<sf::Vector2i>& terrainPath, const sf::Vector2f& worldSize);
private:
	sf::ConvexShape Radar::createEntityShape(const sf::Color bodyColor, const sf::Color outlineColor, const int width, const int height);

	std::vector<sf::ConvexShape> m_terrainLinesShapes;
	std::vector<std::pair<GameObject::Type, sf::Vector2f>> m_gameObjects;
	sf::ConvexShape m_playerShape;
	sf::ConvexShape m_enemyShape;
	sf::ConvexShape m_astronautShape;
	std::vector<float> m_lineWidths;
	sf::FloatRect m_bounds;
	const float X_SCALE = 0.25f;
	const float Y_SCALE = 0.25f;
	const int BOUNDS_BUFFER = 400;
	float m_radarXPos;
};