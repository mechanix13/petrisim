#pragma once

#include "Gate.h"

class AssignGate: public Gate
{
public:
	AssignGate(State* lhs, State* rhs, State* inputSignal);
	~AssignGate();

	void PerformFunc();
};