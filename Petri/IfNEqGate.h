#pragma once

#include "Gate.h"

class IfNEqGate: public Gate
{
public:
    State* TrueSignal;
    State* FalseSignal;

    IfNEqGate(State* lhs, State* rhs, State* inputSignal, State* trueSignal, State* falseSignal);
    ~IfNEqGate();

    void PerformFunc();
    void InitState();
    void UpdateState();
};