#pragma once

#include "State.h"

class Gate
{
public:
	State* InputState;
	State* OutputState;

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