#pragma once

#include "GameObject.h"

class Meteor : public GameObject
{
public:
	Meteor(const sf::Vector2f& startPos, const sf::Vector2f& direction);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	void generateShape();
	std::vector<sf::ConvexShape> m_shapes;	
	std::vector<sf::Vector2f> circle;
	sf::Vector2f m_centre;
	const int METEOR_SEGMENTS = 8;
	const float PI = 3.14159265358979323846f;
};
