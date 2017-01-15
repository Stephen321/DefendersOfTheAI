#include "NestStates.h"

void NWanderState::start(Nest* nest)
{
	std::cout << "started NWanderState" << std::endl;
}

void NWanderState::update(Nest* nest)
{
	std::cout << "update NWanderState" << std::endl;
}

void NWanderState::end(Nest* nest)
{
	std::cout << "end NWanderState" << std::endl;
}
//-----------------------------------------------------------------------------
void NEvadeState::start(Nest* nest)
{
	std::cout << "started Evade" << std::endl;
}

void NEvadeState::update(Nest* nest)
{
	std::cout << "update Evade" << std::endl;
}

void NEvadeState::end(Nest* nest)
{
	std::cout << "end Evade" << std::endl;
}
