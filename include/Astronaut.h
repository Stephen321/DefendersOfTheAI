#pragma once
#include "AI.h"
#include "AstronautStates.h"
#include "Abductor.h"

class Astronaut : public AI<Astronaut>
{
public:
	Astronaut(float startX, const sf::Vector2f& worldSize, const std::vector<sf::Vector2i>& surfacePathPoints);
	void update(float dt) override;
	void checkWorldBounds() override;
	void setBeingAbducted(bool value);
	bool getBeingAbductd() const;
private:
	bool m_beingAbducted; //todo: state for this?
	float getYAtX(float x);
	std::vector<sf::Vector2i> m_surfacePathPoints;
	const float SURFACE_RIM_THICKNESS = 5.f;
	float m_angle;
};