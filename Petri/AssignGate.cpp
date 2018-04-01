#include "AssignGate.h"

AssignGate::AssignGate(State* lhs, State* rhs, State* inputSignal, State* outputSignal, bool inv)
{
    LHS = lhs;
    RHS = rhs;
    InputState = inputSignal;

    inversion = inv;

    OutputState = outputSignal;
}

void AssignGate::PerformFunc()
{
    bool rhsVal = RHS->Condition;
    if (inversion)
    {
        rhsVal = !rhsVal;
    }
    LHS->Condition = rhsVal;

    OutputState->Condition = true;
}

void AssignGate::InitState()
{
    PerformFunc();
}

void AssignGate::UpdateState()
{
    if (InputState->Condition == true)
    {
        PerformFunc();
    }
    else
    {
        OutputState->Condition = false;
    }
}
