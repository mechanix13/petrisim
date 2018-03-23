#pragma once

#define CHECK(a) std::cout << "!!! " << a << " !!!" << std::endl;

enum StateType
{
    Input = 0,
    Intermediate,
    Output
};

enum GateType
{
    Always = 0,
    IfCondition,
    IfAction
};
