#pragma once
#include <memory>
#include "State.h"
class Astronaut;
class AsWanderState : public State<Astronaut>
{
private:
	AsWanderState() {};
public:
	static std::shared_ptr<AsWanderState>& getInstance()
	{
		static std::shared_ptr<AsWanderState> state(new AsWanderState);
		return state;
	}
	void start(Astronaut*) override;
	void update(Astronaut*, float) override;
	void end(Astronaut*) override;
};

class AsFleeState : public State<Astronaut>
{
private:
	AsFleeState() {};
public:
	static std::shared_ptr<AsFleeState>& getInstance()
	{
		static std::shared_ptr<AsFleeState> state(new AsFleeState);
		return state;
	}
	void start(Astronaut*) override;
	void update(Astronaut*, float) override;
	void end(Astronaut*) override;
};

class AsAbductState : public State<Astronaut>
{
private:
	AsAbductState() {};
public:
	static std::shared_ptr<AsAbductState>& getInstance()
	{
		static std::shared_ptr<AsAbductState> state(new AsAbductState);
		return state;
	}
	void start(Astronaut*) override;
	void update(Astronaut*, float) override;
	void end(Astronaut*) override;
};
#include "Astronaut.h"