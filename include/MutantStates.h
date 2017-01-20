//#pragma once
//#include <memory>
//#include "State.h"
//class Mutant;
//class AFlockState : public State<Mutant>
//{
//private:
//	AFlockState() {};
//public:
//	static std::shared_ptr<AFlockState>& getInstance()
//	{
//		static std::shared_ptr<AFlockState> state(new AFlockState);
//		return state;
//	}
//	void start(Mutant*) override;
//	void update(Mutant*, float) override;
//	void end(Mutant*) override;
//};
//
//class ADropState : public State<Mutant>
//{
//private:
//	ADropState() {};
//public:
//	static std::shared_ptr<ADropState>& getInstance()
//	{
//		static std::shared_ptr<ADropState> state(new ADropState);
//		return state;
//	}
//	void start(Mutant*) override;
//	void update(Mutant*, float) override;
//	void end(Mutant*) override;
//};
//
//class APatrolState : public State<Mutant>
//{
//private:
//	APatrolState() {};
//public:
//	static std::shared_ptr<APatrolState>& getInstance()
//	{
//		static std::shared_ptr<APatrolState> state(new APatrolState);
//		return state;
//	}
//	void start(Mutant*) override;
//	void update(Mutant*, float) override;
//	void end(Mutant*) override;
//};
//
//class AAbductingState : public State<Mutant>
//{
//private:
//	AAbductingState() {};
//public:
//	static std::shared_ptr<AAbductingState>& getInstance()
//	{
//		static std::shared_ptr<AAbductingState> state(new AAbductingState);
//		return state;
//	}
//	void start(Mutant*) override;
//	void update(Mutant*, float) override;
//	void end(Mutant*) override;
//};
//
//#include "Mutant.h"