#include "StateHandler.h"
#include "Unit.h"

StateHandler::StateHandler(State* initialState)
{
	currentState = initialState;
	this->unit = unit;
}
