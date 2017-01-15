//#include "FSM.h"
//
//template<typename T>
//void FSM<T>::update()
//{
//	if (m_globalState)
//	{
//		m_globalState->update(m_owner);
//	}
//	if (m_currentState)
//	{
//		m_currentState->update(m_owner);
//	}
//}
//
//template<typename T>
//void FSM<T>::changeState(State<T>* pNewState)
//{
//	assert(pNewState &&
//		"<StateMachine::ChangeState>: trying to change to a null state");
//
//	//call the exit method of the existing state
//	m_currentState->Exit(m_owner);
//
//	//change state to the new state
//	m_currentState = pNewState;
//
//	//call the entry method of the new state
//	m_currentState->Enter(m_owner);
//}
//
//template<typename T>
//void FSM<T>::setCurrentState(State<T>* s)
//{
//	m_currentState = s;
//}
//
//template<typename T>
//void FSM<T>::setGlobalState(State<T>* s)
//{
//	m_globalState = s;
//}
//
