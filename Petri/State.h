#pragma once

#include "common_types.h"
#include <string>

class State
{
public:
    bool Condition;
    std::string Name;

    State(std::string name);
};