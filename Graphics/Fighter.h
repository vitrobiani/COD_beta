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
	int ammo_th;
	bool isReloading, isCallingSquire;
	int reloadTime;
public:
	Fighter(Position start_pos, TeamID tid);
	void engageEnemy(Position enemy_pos);
	void setAmmo(int a) { ammo = a; };
	void setGrenadeCount(int g) { grenade_count = g; };
	void setIsReloading(bool r) { isReloading = r; };
	void setIsCallingSquire(bool c) { isCallingSquire = c; };
	void loadBullet(Position enemy_pos);
	void loadGrenade();
    int getAmmo() const { return ammo; }
    int getGrenadeCount() const { return grenade_count; }
	bool isEnemyInSight(Position enemy_pos);
	void moveToEnemy(Position enemy_pos);
	void addToSquireQueue();
	void checkIfFitForFight();

    const char* getType() override { return "Fighter"; }
    int getAmmo() override { return ammo; }
    int getGrenades() override { return grenade_count; }
};

