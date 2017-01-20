#pragma once
#include <memory>
#include "State.h"
class Mutant;
class MSwarmState : public State<Mutant>
{
private:
	MSwarmState() {};
public:
	static std::shared_ptr<MSwarmState>& getInstance()
	{
		static std::shared_ptr<MSwarmState> state(new MSwarmState);
		return state;
	}
	void start(Mutant*) override;
	void update(Mutant*, float) override;
	void end(Mutant*) override;
};

#include "Mutant.h"