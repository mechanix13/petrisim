#pragma once

#include "Gate.h"

#define MODE_CHANGE 0
#define MODE_POSEDGE 1
#define MODE_NEGEDGE 2

class AlwaysGate: public Gate
{
public:
    State* InputState;
    Gate* Action;
    bool InputStateCond;
    int mode;

    AlwaysGate(State* input, State* output, int _mode);

    void PerformFunc();
    void InitState();
    void UpdateState();
};