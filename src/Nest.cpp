#include "Nest.h"


Nest::Nest(const sf::Vector2f& startPos)
	: AI(startPos, Type::Nest)
{
	m_fsm.init(this);
	m_fsm.changeState(NWanderState::getInstance());
}