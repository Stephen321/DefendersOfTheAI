#pragma once
#include <memory>
#include "State.h"

template <typename T>
class FSM
{
public:

	FSM() :
		m_currentState(0)
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

	void update() 
	{
		if (m_currentState)
		{
			m_currentState->update(m_owner);
		}
	}

	//change to a new state
	void changeState(std::shared_ptr<State<T>> newState)
	{
		if (!newState)
		{
			std::cout << "newState was nullptr" << std::endl;
			return;
		}
		if (!m_currentState)
		{
			m_currentState = newState;
			std::cout << "m_currentState was nullptr" << std::endl;
			m_currentState->start(m_owner);
			return;
		}

		m_currentState->end(m_owner);
		m_currentState = newState;
		m_currentState->start(m_owner);
	}

	//accessors
	std::shared_ptr<State<T>> currentState()  const
	{ 
		return m_currentState;
	};

	//returns true if the current state’s type is equal to the type of the
	//class passed as a parameter.
	//bool  isInState(const State<T>& st)const;

private:
	T* m_owner;
	std::shared_ptr<State<T>> m_currentState;
};
