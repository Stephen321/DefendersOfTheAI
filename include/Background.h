#pragma once
#include "Layer.h"

class Background : public sf::Drawable
{
public:
	Background(const sf::FloatRect& bounds, const std::shared_ptr<GameObject>& player);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt, const sf::FloatRect& cameraBounds);
	void calculateSurfacePoints(const sf::Vector2f& screenSize);
	void createSurface(const sf::Vector2f& screenSize, int screenUnit);
	std::vector<sf::Vector2i> getSurfacePath() const;

private:
	sf::FloatRect m_cameraBounds;
	std::vector<std::unique_ptr<Layer>> m_layers;
	std::vector<sf::ConvexShape> m_surfaceShapes;
	std::vector<sf::Vector2i> pointCoords;
};
