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
	int ammo_th, grenade_th;
	bool isReloading; // determines if the fighter is reloading
	bool isCallingSquire; // determines if the fighter is in the calling squire queue
	bool passive; // determines if the fighter is passive or aggressive meaning it will get closer to enemies
	bool cooprative; // determines if the fighter will set its target according to its teammates target
	int reloadTime;
	Soldier* target;
public:
	Fighter(Position start_pos, TeamID tid);
	~Fighter();
	void engageEnemy();
	void setAmmo(int a) { ammo = a; };
	void setGrenadeCount(int g) { grenade_count = g; };
	void setIsReloading(bool r) { isReloading = r; };
	void setIsCallingSquire(bool c) { isCallingSquire = c; };
	void loadBullet(Position enemy_pos);
	void loadGrenade(Position enemy_pos);
	void defensiveManouver();
	int getAmmo() const { return ammo; };
	int getAmmoTh() const { return ammo_th; };
	int getGrenadeTh() const { return grenade_th; };
	int getGrenadeCount() const { return grenade_count; };
	bool getIsPassive() const { return passive; };
	void setTarget(Soldier* t) { target = t; };
	Soldier* getTarget() { return target; };
	bool getIsCooprative() const { return cooprative; };
	bool isEnemyInSight(Position enemy_pos);
	bool isEnemyInSameRoomAsMe(Position enemy_pos);
	void moveToEnemy();
	void addToSquireQueue();
	void checkIfFitForFight();

    const char* getType() override { return "Fighter"; }
    int getAmmo() override { return ammo; }
    int getGrenades() override { return grenade_count; }
};

