#include "AssignGate.h"

AssignGate::AssignGate(State* lhs, State* rhs, State* inputSignal)
{
    LHS = lhs;
    RHS = rhs;
    InputState = inputSignal;
}

void AssignGate::PerformFunc()
{
    if (InputState->Condition == true)
    {
        LHS->Condition = RHS->Condition;
    }

    OutputState->Condition = true;
}