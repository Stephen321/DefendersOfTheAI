#include "NestStates.h"

void NWanderState::start(Nest* nest)
{
	nest->setProduceTimer(0.f);
	nest->setMoving(true);
}

void NWanderState::update(Nest* nest, float dt)
{
	nest->setPlayerPos();
	if (nest->checkIfReachedTarget())
	{
		nest->getWanderTarget();
	}
	if (nest->playerInRange())
	{
		nest->changeState(NEvadeState::getInstance());
	}
	nest->fire(dt);
	nest->produceAbductors(dt);
}

void NWanderState::end(Nest* nest)
{
	nest->setMoving(false);
}

//-----------------------------------------------------------------------------
void NEvadeState::start(Nest* nest)
{
	nest->setMoving(true);
}

void NEvadeState::update(Nest* nest, float dt)
{
	nest->setPlayerPos();
	nest->evade();
	nest->checkWorldBounds();
	if (nest->playerInRange() == false)
	{
		nest->changeState(NWanderState::getInstance());
	}
	nest->produceAbductors(dt);
	nest->fire(dt);
}

void NEvadeState::end(Nest* nest)
{
	nest->setMoving(false);
	nest->setTargetPos(nest->getPosition());
}
