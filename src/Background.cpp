#include "Background.h"

Background::Background(const sf::FloatRect& bounds)
{
	m_layers.push_back(Layer("assets/sprites/enviroment/stars", bounds, 3, 0.33f));
	m_layers.push_back(Layer("assets/sprites/enviroment/surface", bounds, 9));
	m_worldLayer = &m_layers[1]; //layer which will contain all the gameobjects (surface layer) which has 0.f for m_scrollMultiplier
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const Layer& layer : m_layers)
	{
		target.draw(layer);
	}
}

void Background::update(float worldVelX)
{
	for (Layer& layer : m_layers)
	{
		layer.update(worldVelX);
	}
}

sf::Vector2u Background::getWorldSize() const
{ 
	return m_layers.back().getTotalSectionSize(); //world size is the total section size of the last layer (the surface)
}

void Background::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_worldLayer->addGameObject(gameObject);
}
