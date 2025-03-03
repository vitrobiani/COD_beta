#pragma once
#include "State.h"
#include "Soldier.h"
#include "definitions.h"

class StateSearchEnemy : public State
{
public:
	void OnEnter(Soldier* p);
	void Transition(Soldier* p);
	void OnExit(Soldier* p);

};

