#include "AssignGate.h"

AssignGate::AssignGate(State* lhs, State* rhs, State* inputSignal, State* outputSignal)
{
    LHS = lhs;
    RHS = rhs;
    InputState = inputSignal;

    OutputState = outputSignal;
}

void AssignGate::PerformFunc()
{
    if (InputState->Condition == true)
    {
        LHS->Condition = RHS->Condition;
    }

    OutputState->Condition = true;
}