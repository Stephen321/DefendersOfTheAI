#include "MutantStates.h"

//-----------------------------------------------------------------------------
void MSwarmState::start(Mutant* mutant)
{
	mutant->setMoving(true);
}

void MSwarmState::update(Mutant* mutant, float dt)
{ 
	mutant->setAcceleration(mutant->swarm() + mutant->separation() + mutant->seek());
	mutant->move(dt);
	mutant->fire(dt);
	mutant->checkWorldBounds();
}

void MSwarmState::end(Mutant * mutant)
{
}

//-----------------------------------------------------------------------------
void MAttackState::start(Mutant* mutant)
{
	mutant->setMoving(true);
}

void MAttackState::update(Mutant* mutant, float dt)
{
}

void MAttackState::end(Mutant * mutant)
{
}
