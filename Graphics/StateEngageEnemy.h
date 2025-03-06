#pragma once
#include "State.h"
#include "Fighter.h"
#include "StateCallSquire.h"
#include "Team.h"

class StateEngageEnemy : public State
{
public:
	string toString() { return "Engage Enemy"; }
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);
};

