#include "AlwaysGate.h"
#include <iostream>
#include <sstream>
#include <string>

AlwaysGate::AlwaysGate(State* input, State* output, int _mode)
{
    InputState = input;
    OutputState = output;
    mode = _mode;

    InputStateCond = InputState->Condition;
}

void AlwaysGate::PerformFunc()
{
    OutputState->Condition = true;
    InputStateCond = InputState->Condition;
}

void AlwaysGate::UpdateState()
{
    bool condition = false;
    switch(mode)
    {
        case MODE_CHANGE:
            condition = InputState->Condition != InputStateCond;
            break;
        case MODE_POSEDGE:
            condition = (InputStateCond == false) && (InputState->Condition == true);
            break;
        case MODE_NEGEDGE:
            condition = (InputStateCond == true) && (InputState->Condition == false);
            break;
        default:
            return;
    }
    if (condition)
    {
        PerformFunc();
    }
    else
    {
        OutputState->Condition = false;
    }

    InputStateCond = InputState->Condition;
}

void AlwaysGate::InitState()
{
    PerformFunc();
}
