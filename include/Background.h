#pragma once

#include "Layer.h"

class Background : public sf::Drawable
{
public:
	Background(const sf::FloatRect& bounds);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float worldVelX);
	sf::Vector2u getWorldSize() const;
	void addGameObject(std::shared_ptr<GameObject> gameObject);

private:
	const int LAYERS = 1;
	std::vector<Layer> m_layers;
	Layer* m_worldLayer;
};
