#pragma once
#include "Soldier.h"

class Soldier;
// pure virtual class (interface)

class State
{
public:
	virtual void OnEnter(Soldier* p) = 0;
	virtual void Transition(Soldier* p) = 0;
	virtual void OnExit(Soldier* p) = 0;
};


