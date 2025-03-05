#pragma once
#include "State.h"
#include "Fighter.h"
#include "StateSearchEnemy.h"
#include "Team.h"

class StateCallSquire : public State
{
public:
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);
};

