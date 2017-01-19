#include "AstronautStates.h"

void AsWanderState::start(Astronaut* astronaut)
{
	astronaut->setMoving(true);
	astronaut->updateAcceleration();
}

void AsWanderState::update(Astronaut* astronaut, float dt)
{

	astronaut->setPosition(sf::Vector2f(astronaut->getPosition().x, astronaut->getYAtX(astronaut->getPosition().x)));
	astronaut->checkWorldBounds();
	if (astronaut->getBeingChased())
	{
		astronaut->changeState(AsFleeState::getInstance());
	}
}

void AsWanderState::end(Astronaut* astronaut)
{
}

//-----------------------------------------------------------------------------
void AsFleeState::start(Astronaut* astronaut)
{
	int fleeDir;
	if (astronaut->getAbductor()->getPosition().x > astronaut->getPosition().x)
	{
		fleeDir = -1;
	}
	else
	{
		fleeDir = 1;
	}
	astronaut->setDirection(sf::Vector2f((float)fleeDir, 0.f));
	astronaut->updateAcceleration();
}

void AsFleeState::update(Astronaut* astronaut, float dt)
{
	astronaut->setPosition(sf::Vector2f(astronaut->getPosition().x, astronaut->getYAtX(astronaut->getPosition().x)));
	int fleeDir;
	if (astronaut->getAbductor()->getPosition().x > astronaut->getPosition().x)
	{
		fleeDir = -1;
	}
	else
	{
		fleeDir = 1;
	}
	astronaut->setDirection(sf::Vector2f((float)fleeDir, 0.f));
	astronaut->updateAcceleration();
	if (astronaut->getBeingAbducted())
	{
		astronaut->changeState(AsAbductState::getInstance());
	}
	astronaut->checkWorldBounds();
}

void AsFleeState::end(Astronaut * astronaut)
{
}

//-----------------------------------------------------------------------------
void AsAbductState::start(Astronaut* astronaut)
{
	astronaut->setBeingChased(false);
}

void AsAbductState::update(Astronaut* astronaut, float dt)
{
	astronaut->seekAbductionPos();
	if (astronaut->getBeingAbducted() == false)
	{
		//TODO: change to fall state
	}
	astronaut->move(dt);
	astronaut->checkWorldBounds();
}

void AsAbductState::end(Astronaut * astronaut)
{
}