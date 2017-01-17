#pragma once
#include "Layer.h"

class Background : public sf::Drawable
{
public:
	Background(const sf::FloatRect& bounds, const std::shared_ptr<GameObject>& player, const std::vector<std::shared_ptr<GameObject>>& gameObjects);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
private:
	std::vector<std::unique_ptr<Layer>> m_layers;
};
