#pragma once
#include <iostream>
#include "StateHandler.h"

class StateHandler_Gatherer : public StateHandler
{
public:
	//TODO -> Document this entire badboy

	StateHandler_Gatherer(): 
		StateHandler(roamState)
	{
		if(!hasBeenInitialized)
			InitializeStates();
	}

	static State* roamState;
	static State* gatherState;
	static State* fleeState;
	static bool hasBeenInitialized;

	static void InitializeStates()
	{
		StateHandler_Gatherer::hasBeenInitialized = true;
		StateHandler_Gatherer::roamState->Copy(new State(
			std::list<Transition*> {new Transition(StateHandler_Gatherer::fleeState)},
			DebugRoam
		));

		StateHandler_Gatherer::gatherState->Copy( new State(
			std::list<Transition*> {new Transition(StateHandler_Gatherer::roamState)},
			DebugGather
		));

		StateHandler_Gatherer::fleeState->Copy( new State(
			std::list<Transition*> {new Transition(StateHandler_Gatherer::gatherState)},
			DebugFlee
		));
	}

	static void DebugRoam()
	{
		std::cout<<"ROAM" <<std::endl;
	}
	static void DebugFlee()
	{
		std::cout<<"Flee" <<std::endl;
	}
	static void DebugGather()
	{
		std::cout << "Gather" << std::endl;
	}

};