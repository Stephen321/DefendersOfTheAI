#pragma once
#include "AI.h"
#include "AstronautStates.h"
#include "Abductor.h"

class Astronaut : public AI<Astronaut>
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Astronaut(const sf::Vector2f& startPos, const sf::Vector2f& worldSize, GameObjectPtrVector& surfacePathPoints);
private:
	GameObjectPtrVector& m_surfacePathPoints;
};