#pragma once
#include "State.h"
#include "Transition.h"

class StateHandler
{
public:
	State* currentState;

	StateHandler(State* initialState)
	{
		currentState = initialState;
	}

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