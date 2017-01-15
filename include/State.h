#pragma once

template <typename T>
class State
{
public:
	virtual void start(T*) = 0;
	virtual void update(T*) = 0;
	virtual void end(T*) = 0;
	virtual ~State() = default;
	State(State const&) = delete;             // Copy construct
	State(State&&) = delete;                  // Move construct
	State& operator=(State const&) = delete;  // Copy assign
	State& operator=(State &&) = delete;      // Move assign
protected:
	State() {};
};
