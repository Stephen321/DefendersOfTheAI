#pragma once
#include <memory>
#include "State.h"

template <typename T>
class FSM
{
public:

	FSM() 
		: m_currentState(0)
	{};

	void init(T* owner)
	{
		m_owner = owner;
	}
	//use these methods to initialize the FSM
	void setCurrentState(std::shared_ptr<State<T>> s) 
	{
		m_currentState = s;
	}

	void update(float dt)
	{
		if (m_currentState)
		{
			m_currentState->update(m_owner, dt);
		}
	}

	//change to a new state
	void changeState(std::shared_ptr<State<T>> newState)
	{
		if (!newState)
		{
			return;
		}
		if (!m_currentState)
		{
			m_currentState = newState;
			m_currentState->start(m_owner);
			return;
		}

		m_currentState->end(m_owner);
		m_currentState = newState;
		m_currentState->start(m_owner);
	}

	std::shared_ptr<State<T>> currentState()  const
	{ 
		return m_currentState;
	};

private:
	T* m_owner;
	std::shared_ptr<State<T>> m_currentState;
};
