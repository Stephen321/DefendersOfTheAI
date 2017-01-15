#include "NestStates.h"

void NWanderState::start(Nest* nest)
{
	nest->setMoving(true);
}

void NWanderState::update(Nest* nest)
{
	if (nest->checkIfReachedTarget())
	{
		nest->getWanderTarget();
	}
	if (nest->playerInRange())
	{
		nest->changeState(NEvadeState::getInstance());
	}
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

void NEvadeState::update(Nest* nest)
{
	nest->evade();
	if (nest->playerInRange() == false)
	{
		nest->changeState(NWanderState::getInstance());
	}
	nest->fire();
}

void NEvadeState::end(Nest* nest)
{
	nest->setMoving(false);
	nest->setTargetPos(nest->getPosition());
}
