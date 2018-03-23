#pragma once

#include "Gate.h"

// TODO: remove this and make a gate type for every operation

class ActionGate: public Gate
{
public:
    std::string Action; 

    ActionGate(State* input, State* output, std::string action);
    ~ActionGate();

    void PerformFunc();
};