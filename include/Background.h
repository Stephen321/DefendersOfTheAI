#pragma once

#include "Layer.h"
#include <deque>

class Background : public sf::Drawable
{
public:
	Background(const sf::FloatRect& bounds, const std::shared_ptr<GameObject>& player, const std::vector<std::shared_ptr<GameObject>>& gameObjects);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	void calculateSurfacePoints(const sf::Vector2f& screenSize);
	void createSurface(const sf::Vector2f& screenSize, int screenUnit);

private:
	const int LAYERS = 1;
	std::vector<std::unique_ptr<Layer>> m_layers;
	std::deque<sf::ConvexShape> m_surfaceShapes;
	std::deque<sf::ConvexShape> m_surfaceRimShapes;
	std::vector<sf::Vector2i> pointCoords;
};
