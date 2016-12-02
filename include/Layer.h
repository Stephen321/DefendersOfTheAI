#pragma once

#include <iostream> //debug
#include <memory>
#include "Helpers.h"
#include "GameObject.h"

class Layer : public sf::Drawable
{
public:
	Layer(const std::string& path, const sf::FloatRect& bounds, int sections, float scrollMultiplier = 0.f);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float worldVelX);
	sf::Vector2u getTotalSectionSize() const;
	void addGameObject(std::shared_ptr<GameObject> gameObject);

private:
	struct Section
	{
		sf::Texture texture;
		int width;
		int height;
		sf::Sprite sprite;
		sf::RectangleShape debugShape;
		sf::Font debugFont;
		sf::Text debugText;
	};
	std::vector<std::shared_ptr<GameObject>> m_gameObjects; //store pointer to game objects so we can teleport them to either end of the world for seamless wrap around
	int m_teleportSection;
	bool m_teleported;

	const int SECTIONS;
	std::vector<Section> m_sections;
	const sf::FloatRect& m_bounds;
	int m_left, m_middle, m_right;
	float m_scrollMultiplier;
	bool m_scrollable;

	void positionSection(int section, int direction);
	void teleport(int section, int direction, int sectionLocation);
};
