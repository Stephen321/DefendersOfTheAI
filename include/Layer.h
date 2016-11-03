#ifndef LAYER_H
#define LAYER_H

#include "SFML\Graphics.hpp"

class Layer : public sf::Drawable
{
public:
	Layer(const std::string& path, const sf::Vector2f& screenSize, int sections, float scrollMultiplier);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(const sf::Vector2f& worldVelocity);

private:
	struct Section
	{
		sf::Texture texture;
		sf::Sprite sprite;
	};
	const int SECTIONS;
	std::vector<Section> m_sections;
	sf::Vector2f m_screenSize;
	int m_left, m_middle, m_right;
	int m_sectionWidth;
	int m_sectionHeight;
	float m_scrollMultiplier;

	void setPositions();
	int clamp(int value);
};

#endif 
