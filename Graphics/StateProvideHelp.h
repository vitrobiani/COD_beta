#pragma once
#include "State.h"
#include "Soldier.h"
#include "Fighter.h"
#include "definitions.h"
#include "Team.h"

class StateProvideHelp: public State
{
private:
	bool restock = false;
public:
	string toString() { return "ProvideHelp"; }
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);

};
