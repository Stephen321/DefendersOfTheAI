#include "AbductorStates.h"

void AFlockState::start(Abductor* abductor)
{
	abductor->setMoving(true);
	abductor->resetMaxVelocity();
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
}

void AFlockState::end(Abductor* abductor)
{
}

//-----------------------------------------------------------------------------
void APatrolState::start(Abductor* abductor)
{
	abductor->setMoving(true);
	abductor->setDirection(sf::Vector2f(-1, 0));// sf::Vector2f((rand() % 2 == 0) ? -1.f : 1.f, 0.f));
	abductor->setAcceleration(abductor->getForceAmount() * abductor->getDirection());
	abductor->setMaxPatrolVelocity();
}

void APatrolState::update(Abductor* abductor, float dt)
{ //TODO: this has a fixed speed not set from anywhere (player x moves at max vel * 0.5 and y is sin way
	int neighbourCount = abductor->getNeighbourCount();
	if (neighbourCount > 0)
	{
		//abductor->changeState(AFlockState::getInstance());
	}
	abductor->move(dt);
	abductor->setYPosWave();
}

void APatrolState::end(Abductor * abductor)
{
}
