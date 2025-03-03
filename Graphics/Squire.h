#pragma once
#include "Soldier.h"

class Squire : public Soldier
{
private:
	int ammo, grenade_count, health_pack;
public:
	Squire(Position start_pos, TeamID tid);
};

