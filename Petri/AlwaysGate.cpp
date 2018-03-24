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
    OutputState->Condition = true;
    InputStateCond = InputState->Condition;
}

void AlwaysGate::UpdateState()
{
    if (InputState->Condition != InputStateCond)
    {
        PerformFunc();
    }
    else
    {
        OutputState->Condition = false;
    }
}

void AlwaysGate::InitState()
{
    PerformFunc();
}
