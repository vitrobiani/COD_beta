#pragma once
#include "definitions.h"
#include "State.h"
#include "Soldier.h"
#include "StateSearchEnemy.h"

class Fighter: public Soldier 
{
private:
	int ammo, grenade_count;
public:
	Fighter(Position start_pos, TeamID tid);
};

