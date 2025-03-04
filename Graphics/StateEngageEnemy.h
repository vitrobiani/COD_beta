#pragma once
#include "State.h"
#include "Fighter.h"
#include "StateCallSquire.h"

class StateEngageEnemy : public State
{
public:
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);
};

