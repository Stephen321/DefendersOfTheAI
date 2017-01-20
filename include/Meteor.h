#pragma once

#include "GameObject.h"

class Meteor : public GameObject
{
public:
	Meteor(const sf::Vector2f& worldSize, const int radius);
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual sf::FloatRect getRect() const override;
	void checkWorldBounds() override;
	float getHeight() const override;
private:
	void generateShape(sf::Vector2f worldSize, int);
	void createConvexCircle(int, int, int, sf::Color);

	//number of sides in the shape generated to form the meteor
	const int METEOR_FACES = 8;

	//an array of values in degrees representing the range each colour should appear in
	const std::pair<float, float> ANGLE_RANGES[8] = { 
		{ 337.5f, 22.5f  },  //darkest
		{  22.5f, 67.5f  },  //dark
		{  67.5f, 112.5f },  //shaded
		{ 112.5f, 157.5f },  //midtone
		{ 157.5f, 202.5f },  //highlight
		{ 202.5f, 247.5f },  //midtone
		{ 247.5f, 292.5f },  //shaded
		{ 292.5f, 337.5f }}; //dark

	const int ANGLE_BETWEEN_POINTS = 45;

	std::vector<sf::ConvexShape> m_shapes;
	sf::Vector2f m_centre;
	sf::Color m_colors[5];
	
	const int m_radius;
	
};
