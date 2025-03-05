#pragma once
#include "definitions.h"
#include "State.h"
#include "Soldier.h"
#include "StateSearchEnemy.h"
#include "Grenade.h"

class Fighter: public Soldier 
{
private:
	int ammo, grenade_count;
	bool isReloading;
	int reloadTime;
public:
	Fighter(Position start_pos, TeamID tid);
	void engageEnemy(Position enemy_pos);
	void setAmmo(int a) { ammo = a; };
	void setGrenadeCount(int g) { grenade_count = g; };
	void setIsReloading(bool r) { isReloading = r; };
	void loadBullet(Position enemy_pos);
	void loadGrenade();
    int getAmmo() const { return ammo; }
    int getGrenadeCount() const { return grenade_count; }

    const char* getType() override { return "Fighter"; }
    int getAmmo() override { return ammo; }
    int getGrenades() override { return grenade_count; }
};

