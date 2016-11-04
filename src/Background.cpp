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

void Background::update(const sf::Vector2f & worldVelocity)
{
	for (Layer& layer : m_layers)
	{
		layer.update(worldVelocity);
	}
}

sf::Vector2u Background::getWorldSize()
{ //world width is the number of sections in the surface layer times the width of each
	return sf::Vector2u(m_layers.back().getSectionWidth() * m_layers.back().getSectionCount(), m_layers.back().getSectionHeight());
}
