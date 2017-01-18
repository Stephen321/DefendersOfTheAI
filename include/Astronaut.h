#pragma once
#include "AI.h"
#include "AstronautStates.h"
#include "Abductor.h"

class Astronaut : public AI<Astronaut>
{
public:
	Astronaut(const sf::Vector2f& startPos, const sf::Vector2f& worldSize);
};