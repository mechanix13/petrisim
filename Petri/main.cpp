#include "common.h"

int main()
{
    std::string test = "Hello\n";
    std::cout << test << std::endl;

    TrueCondition = new State("true");
    TrueCondition->Condition = true;

    FalseCondition = new State("false");
    FalseCondition->Condition = false;

    ReadFile("reg.v");

    Model(200, 1);

    return 0;
}
