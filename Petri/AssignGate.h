#pragma once

#include "Gate.h"

class AssignGate: public Gate
{
public:
    bool inversion;

    AssignGate(State* lhs, State* rhs, State* inputSignal, State* outputSignal, bool inv);
    ~AssignGate();

    void PerformFunc();
    void InitState();
    void UpdateState();
};