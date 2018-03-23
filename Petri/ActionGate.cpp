#include "ActionGate.h"
#include <iostream>
#include <sstream>
#include <string>

ActionGate::ActionGate(State* input, State* output, std::string action = "")
{
    Action = action;
}

ActionGate::~ActionGate()
{
}

void ActionGate::PerformFunc()
{
    bool funcVal;
    std::stringstream act;
    std::string buf;

    int newVal = 0;

    act.str(Action.c_str());
    act >> buf >> buf >> newVal;
    OutputState->Condition = newVal;
}
