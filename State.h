#pragma once
#include <list>

class Transition;

class State
{

public:
	State(std::list<Transition*> transitions, void (* action)()):transitions(transitions),action(action){}
	State() {};
	void Copy(State* stateToCopy)
	{
		action = stateToCopy->action;
		transitions = stateToCopy->transitions;
	}
	std::list<Transition*> transitions;
	void (* action)();
	void Act()
	{
		action();
	}
};