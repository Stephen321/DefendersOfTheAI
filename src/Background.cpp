#include "Background.h"
//TODO: good to use shared_ptr here?
Background::Background(const sf::FloatRect& bounds, const std::shared_ptr<GameObject>& player, const std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
	m_layers.push_back(std::make_unique<Layer>(Layer("assets/sprites/enviroment/stars", bounds, 3, player, -0.333333f)));
	m_layers.push_back(std::make_unique<Layer>(Layer("assets/sprites/enviroment/mountains", bounds, 7, player, -0.666666f)));
	m_layers.push_back(std::make_unique<Layer>(Layer("assets/sprites/enviroment/surface", bounds, 9, player, NULL, gameObjects)));
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const std::unique_ptr<Layer>& layer : m_layers)
	{
		target.draw(*layer);
	}
}

void Background::update(float dt)
{
	for (std::unique_ptr<Layer>& layer : m_layers)
	{
		layer->update(dt);
	}
}