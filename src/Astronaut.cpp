#include "Astronaut.h"


Astronaut::Astronaut(const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
	: AI(Type::Astronaut, startPos, worldSize)
{
	m_fsm.init(this);
	m_fsm.changeState(0);//TODO: put astronaut states in here
}
