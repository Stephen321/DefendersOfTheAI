#pragma once

#include "Layer.h"

class Background : public sf::Drawable
{
public:
	Background(const sf::Vector2f& screenSize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(const sf::Vector2f& worldVelocity);
	sf::Vector2u getWorldSize();

private:
	const int LAYERS = 1;
	std::vector<Layer> m_layers;
};
