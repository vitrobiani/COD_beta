#pragma once
#include "State.h"
class StateEngageEnemy : public State
{
public:
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);
};

