#pragma once

#include "Layer.h"
#include <deque>

class Background : public sf::Drawable
{
public:
	Background(const sf::Vector2f& screenSize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float worldVelX);
	void calculateSurfacePoints(const sf::Vector2f& screenSize);
	void createSurface(const sf::Vector2f& screenSize, int screenUnit);
	sf::Vector2u getWorldSize() const;

private:
	const int LAYERS = 1;
	std::vector<Layer> m_layers;
	std::deque<sf::ConvexShape> m_surfaceShapes;
	std::deque<sf::ConvexShape> m_surfaceRimShapes;
	std::vector<sf::Vector2i> pointCoords;
};
