#include "IfNeqGate.h"
#include <iostream>
#include <sstream>
#include <string>

IfNEqGate::IfNEqGate(State* lhs, State* rhs, State* inputSignal, State* trueSignal, State* falseSignal)
{
    LHS = lhs;
    RHS = rhs;

    InputState = inputSignal;

    TrueSignal = trueSignal;
    FalseSignal = falseSignal;
}

IfNEqGate::~IfNEqGate()
{
}

void IfNEqGate::PerformFunc()
{
    if (LHS->Condition != RHS->Condition)
    {
        TrueSignal->Condition = true;
        FalseSignal->Condition = false;
    }
    else
    {
        FalseSignal->Condition = true;
        TrueSignal->Condition = false;
    }
}

void IfNEqGate::InitState()
{
    PerformFunc();
}

void IfNEqGate::UpdateState()
{
    if (InputState->Condition == true)
    {
        PerformFunc();
    }
    else
    {
        TrueSignal->Condition = false;
        FalseSignal->Condition = false;
    }
}
