#pragma once

#include "Gate.h"

class IfEqGate: public Gate
{
public:
    State* TrueSignal;
    State* FalseSignal;
    IfEqGate(State* lhs, State* rhs, State* inputSignal, State* trueSignal, State* falseSignal);
    ~IfEqGate();

    void PerformFunc();
    void InitState();
    void UpdateState();
};