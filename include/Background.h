#pragma once

#include "Layer.h"

class Background : public sf::Drawable
{
public:
	Background(const sf::Vector2f& screenSize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float worldVelX);
	void generateSurface(const sf::Vector2f& screenSize);
	sf::Vector2u getWorldSize() const;

private:
	const int LAYERS = 1;
	std::vector<Layer> m_layers;
	std::vector<sf::ConvexShape> m_surfaceShapes;
};
