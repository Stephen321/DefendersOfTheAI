#pragma once

#include <memory>
#include "Helpers.h"
#include "GameObject.h"
#include "Constants.h"

class Layer : public sf::Drawable
{
public:
	Layer(const std::string& path, const sf::FloatRect& bounds, int sections, const std::shared_ptr<GameObject> player, float scrollMultiplier = 0.f);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
private:
	virtual void positionSection(int section, int direction);

	const int SECTIONS;
	struct Section
	{
		sf::Texture texture;
		sf::Sprite sprite;
	};
	std::vector<Section> m_sections;
	int m_left, m_middle, m_right;
	const sf::FloatRect& m_bounds;
	std::shared_ptr<GameObject> m_player;
	sf::Vector2f m_lastPlayerPos;
	float m_scrollMultiplier;
	std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	bool m_scrollable;
};