#pragma once
#include "State.h"
#include "Transition.h"

class Unit;

class StateHandler
{
public:
	Unit* unit;
	State* currentState;

	StateHandler::StateHandler(State* initialState);

	void HandleTransitions()
	{
		std::list<Transition*>::iterator curTransition;
		for (curTransition = currentState->transitions.begin(); 
			curTransition != currentState->transitions.end();
			curTransition++)
		{
			if ((*curTransition)->ShouldChangeState())
			{
				currentState = (*curTransition)->nextState;
				break;
			}
		}
	}

};