#include "Background.h"

Background::Background(const sf::Vector2f& screenSize)
{
	m_layers.push_back(Layer("assets/sprites/enviroment/stars", screenSize, 3, 0.33f));
	m_layers.push_back(Layer("assets/sprites/enviroment/surface", screenSize, 9, 1.f));
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
