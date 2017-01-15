//#pragma once
//#include "StateType.h"
//
//template<typename T>
//class StateMap { 
//public:
//	State<T>* getState(StateType type)
//	{
//		if (states.find(type) == states.end())
//		{
//			states.insert(std::pair<StateType, State<T>*>(type, nullptr));
//		}
//		return states.at(type);
//	}
//	void add(StateType type, State<T>* stateP)
//	{
//		if (states.find(type) != states.end())
//			return; //dont insert if already exists
//		states.insert(std::pair<StateType, State<T>*>(type, stateP));
//	}
//private:
//	std::unordered_map<StateType, State<T>*> states;
//};
