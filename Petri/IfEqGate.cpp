#include "IfEqGate.h"
#include <iostream>
#include <sstream>
#include <string>

IfEqGate::IfEqGate(State* lhs, State* rhs, State* inputSignal, State* trueSignal, State* falseSignal)
{
    LHS = lhs;
    RHS = rhs;

    InputState = inputSignal;

    TrueSignal = trueSignal;
    FalseSignal = falseSignal;
}

IfEqGate::~IfEqGate()
{
}

void IfEqGate::PerformFunc()
{
    if (LHS->Condition == RHS->Condition)
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

void IfEqGate::InitState()
{
    PerformFunc();
}

void IfEqGate::UpdateState()
{
    if (InputState->Condition == true)
    {
        PerformFunc();
    }
}
