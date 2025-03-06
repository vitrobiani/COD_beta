#pragma once
#include "State.h"
#include "Soldier.h"
#include "definitions.h"
#include "StateEngageEnemy.h"
#include "Team.h"

class StateSearchEnemy : public State
{
public:
	string toString() { return "StateSearchEnemy"; }
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);

};

