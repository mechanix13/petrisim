#pragma once

#include "Gate.h"

class AssignGate: public Gate
{
public:
    AssignGate(State* lhs, State* rhs, State* inputSignal, State* outputSignal);
    ~AssignGate();

    void PerformFunc();
    void InitState();
    void UpdateState();
};