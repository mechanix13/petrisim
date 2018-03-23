#pragma once

#include "Gate.h"

class IfEqGate: public Gate
{
public:
	State* TrueSignal;
	State* FalseSignal;
	IfEqGate(State* lhs, State* rhs, State* trueSignal, State* falseSignal);
	~IfEqGate();

	void PerformFunc();
};