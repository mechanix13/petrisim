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
    LHS->Condition = RHS->Condition;

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
}
