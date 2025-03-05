#pragma once
#include "Soldier.h"

class Squire : public Soldier
{
private:
	int ammo, grenade_count, health_pack;
public:
	Squire(Position start_pos, TeamID tid);
    int getHealthPack() const { return health_pack; }
    const char* getType() override { return "Squire"; }
    int getAmmo() override { return ammo; }
    int getGrenades() override { return grenade_count; }
};

