#include "StateHandler_Gatherer.h"

State* StateHandler_Gatherer::roamState = new State();
State* StateHandler_Gatherer::gatherState = new State();
State* StateHandler_Gatherer::fleeState = new State();
bool StateHandler_Gatherer::hasBeenInitialized = false;
