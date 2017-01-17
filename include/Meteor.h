#pragma once

#include "GameObject.h"

class Meteor : public GameObject
{
public:
	Meteor(const sf::Vector2f& startPos, const sf::Vector2f& worldSize);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool getAlive() const;
private:
	void generateShape(sf::Vector2f worldSize, int);
	std::vector<sf::ConvexShape> m_shapes;	

	void createConvexCircle(int, int, int, sf::Color);



	sf::Vector2f m_centre;
	const int METEOR_SEGMENTS = 8;
	const float PI = 3.14159265358979323846f;
	float SCREEN_HEIGHT;
	int m_radius;
	bool m_alive = true;
};
