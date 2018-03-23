#pragma once

#include "State.h"

class Gate
{
public:
	State* InputState;
	State* OutputState;

    // dirty hack for 'if'-gates
    // otherwise it's hard to extract needed states
    State* TrueSignal;
    State* FalseSignal;

	State* LHS;
	State* RHS;
	bool InputStateCond;

	Gate();
	Gate(State* output);
	~Gate();

	virtual void PerformFunc();
	void UpdateState();
	void InitState();
};