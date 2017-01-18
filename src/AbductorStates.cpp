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
		abductor->changeState(ADropState::getInstance());
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
	abductor->checkWorldBounds();
	abductor->fire(dt);
}

void AFlockState::end(Abductor* abductor)
{
}

//-----------------------------------------------------------------------------
void ADropState::start(Abductor* abductor)
{
	abductor->setMoving(true);
}

void ADropState::update(Abductor* abductor, float dt)
{ //TODO: this has a fixed speed not set from anywhere (player x moves at max vel * 0.5 and y is sin way
	abductor->updateDropAcceleration();
	abductor->move(dt);
	if (abductor->reachedTarget())
	{
		abductor->changeState(APatrolState::getInstance());
	}
}

void ADropState::end(Abductor * abductor)
{
}

//-----------------------------------------------------------------------------
void APatrolState::start(Abductor* abductor)
{
	abductor->setMoving(true);
	abductor->setDirection(sf::Vector2f((abductor->getDirection().x > 0.f) ? 1.f : -1.f, 0.f));
}

void APatrolState::update(Abductor* abductor, float dt)
{ //TODO: this has a fixed speed not set from anywhere (player x moves at max vel * 0.5 and y is sin way
	int neighbourCount = abductor->getNeighbourCount();
	if (neighbourCount > 0)
	{
		abductor->changeState(AFlockState::getInstance());
	}
	abductor->updatePatrolAcceleration();
	abductor->move(dt);
	abductor->checkWorldBounds();
}

void APatrolState::end(Abductor * abductor)
{
}
