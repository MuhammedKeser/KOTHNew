#pragma once
#include "State.h"
#include "InputManager.h"

class Transition
{
public:
	Transition(State* nextState):nextState(nextState) {};
	bool ShouldChangeState()
	{
		if (Input::KeyPressed(InputKeys::KEY::A))
			return true;

		return false;
	}
	State* nextState;

};