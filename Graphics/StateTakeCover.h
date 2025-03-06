#pragma once
#include "State.h"
#include "Squire.h"
#include "Team.h"
#include "StateProvideHelp.h"

class StateTakeCover: public State
{
public:
	string toString() { return "StateTakeCover"; }
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);

};
