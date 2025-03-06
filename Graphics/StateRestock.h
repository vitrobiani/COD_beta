#pragma once
#include "State.h"
#include "Squire.h"


class StateRestock: public State
{
public:
	string toString() { return "Restock"; }
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);

};

