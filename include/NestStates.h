#pragma once
#include <memory>
#include "State.h"
class Nest;
class NWanderState : public State<Nest>
{
private:
	NWanderState() {};
public:
	static std::shared_ptr<NWanderState>& getInstance()
	{
		static std::shared_ptr<NWanderState> state(new NWanderState);
		return state;
	}
	void start(Nest*) override;
	void update(Nest*, float) override;
	void end(Nest*) override;
};

class NEvadeState : public State<Nest>
{
private:
	NEvadeState() {};
public:
	static std::shared_ptr<NEvadeState>& getInstance()
	{
		static std::shared_ptr<NEvadeState> state(new NEvadeState);
		return state;
	}
	void start(Nest*) override;
	void update(Nest*, float) override;
	void end(Nest*) override;
};
#include "Nest.h"