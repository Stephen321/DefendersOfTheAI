#pragma once

#include "GameObject.h"

class Meteor : public GameObject
{
public:
	Meteor(const sf::Vector2f& worldSize);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void generateShape(sf::Vector2f worldSize, int);
	void createConvexCircle(int, int, int, sf::Color);

	std::vector<sf::ConvexShape> m_shapes;
	sf::Vector2f m_centre;
	sf::Color m_colors[5];

	const int METEOR_SEGMENTS = 8;
	const float PI = 3.14159265358979323846f;
	float SCREEN_HEIGHT;
	int m_radius;
	
};
