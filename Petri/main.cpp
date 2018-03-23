#include "common.h"

int main()
{
    std::string test = "Hello\n";
    std::cout << test << std::endl;

    TrueCondition = new State("true");
    TrueCondition->Condition = true;

    FalseCondition = new State("false");
    FalseCondition->Condition = false;

    ReadFile("inv.v");

    /*Model(50, 1);

    delete in;
    delete out;
    delete action;*/

    return 0;
}
