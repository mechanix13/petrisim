#include "Gate.h"
#include <iostream>
#include <sstream>
#include <string>

Gate::Gate(){}

Gate::Gate(State* output)
{
    OutputState = output;
}

Gate::~Gate()
{
}

void Gate::PerformFunc()
{
}

void Gate::UpdateState()
{
}

void Gate::InitState()
{
    PerformFunc();
}
