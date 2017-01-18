#pragma once
#include <memory>
#include "State.h"
class Abductor;
class AFlockState : public State<Abductor>
{
private:
	AFlockState() {};
public:
	static std::shared_ptr<AFlockState>& getInstance()
	{
		static std::shared_ptr<AFlockState> state(new AFlockState);
		return state;
	}
	void start(Abductor*) override;
	void update(Abductor*, float) override;
	void end(Abductor*) override;
};

class ADropState : public State<Abductor>
{
private:
	ADropState() {};
public:
	static std::shared_ptr<ADropState>& getInstance()
	{
		static std::shared_ptr<ADropState> state(new ADropState);
		return state;
	}
	void start(Abductor*) override;
	void update(Abductor*, float) override;
	void end(Abductor*) override;
};

class APatrolState : public State<Abductor>
{
private:
	APatrolState() {};
public:
	static std::shared_ptr<APatrolState>& getInstance()
	{
		static std::shared_ptr<APatrolState> state(new APatrolState);
		return state;
	}
	void start(Abductor*) override;
	void update(Abductor*, float) override;
	void end(Abductor*) override;
};

#include "Abductor.h"