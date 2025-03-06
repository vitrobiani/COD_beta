#pragma once
#include "State.h"
#include "Squire.h"
#include "Team.h"
#include "StateTakeCover.h"

class StateFindTeammates: public State
{
public:
	string toString() { return "StateFindTeammates"; }
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);

};


