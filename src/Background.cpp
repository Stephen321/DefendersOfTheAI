#include "Background.h"

Background::Background(const sf::Vector2f& screenSize)
{
	m_layers.push_back(Layer("assets/sprites/enviroment/stars", screenSize, 5, 1.f));
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const Layer& layer : m_layers)
	{
		target.draw(layer);
	}
}

void Background::update(const sf::Vector2f & worldVelocity)
{
	for (Layer& layer : m_layers)
	{
		layer.update(worldVelocity);
	}
}
