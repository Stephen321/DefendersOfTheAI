#include "AbductorStates.h"

void AFlockState::start(Abductor* abductor)
{
	abductor->setMoving(true);
}

void AFlockState::update(Abductor* abductor, float dt)
{
	int neighbourCount = abductor->getNeighbourCount();
	if (neighbourCount == 0)
	{
		abductor->changeState(APatrolState::getInstance());
	}
	sf::Vector2f sep = abductor->separation();
	sf::Vector2f ali = abductor->alignment();
	sf::Vector2f coh = abductor->cohesion();
	sf::Vector2f acceleration;
	acceleration += sep;
	acceleration += ali;
	acceleration += coh;
	abductor->setAcceleration(acceleration);
	abductor->move(dt);
	abductor->checkBounds();
}

void AFlockState::end(Abductor* abductor)
{
}

//-----------------------------------------------------------------------------
void APatrolState::start(Abductor* abductor)
{
	abductor->setMoving(true);
}

void APatrolState::update(Abductor* abductor, float dt)
{
	int neighbourCount = abductor->getNeighbourCount();
	if (neighbourCount > 0)
	{
		abductor->changeState(AFlockState::getInstance());
	}
	abductor->patrolMove(dt);
}

void APatrolState::end(Abductor * abductor)
{
}
