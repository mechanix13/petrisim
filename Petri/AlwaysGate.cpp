#include "AlwaysGate.h"
#include <iostream>
#include <sstream>
#include <string>

AlwaysGate::AlwaysGate(State* input, State* output)
{
	InputState = input;
	OutputState = output;

	InputStateCond = InputState->Condition;
}

void AlwaysGate::PerformFunc()
{
	Action->PerformFunc();
}

void AlwaysGate::UpdateState()
{
	if (InputState->Condition != InputStateCond)
	{
		OutputState->Condition = InputState->Condition;
		PerformFunc();
		InputStateCond = InputState->Condition;
	}
}

void AlwaysGate::InitState()
{
	Action->InitState();
}
