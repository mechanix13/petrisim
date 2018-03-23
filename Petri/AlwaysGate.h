#pragma once

#include "Gate.h"

class AlwaysGate: public Gate
{
public:
    State* InputState;
    Gate* Action;
    bool InputStateCond;

    AlwaysGate(State* input, State* output);

    void PerformFunc();
    void InitState();
    void UpdateState();
};