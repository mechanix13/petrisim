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
}

void AlwaysGate::UpdateState()
{
    if (InputState->Condition != InputStateCond)
    {
        OutputState->Condition = true;
        InputStateCond = InputState->Condition;
    }
    else
    {
        OutputState->Condition = false;
    }
}

void AlwaysGate::InitState()
{
    OutputState->Condition = true;
    InputStateCond = InputState->Condition;
}
