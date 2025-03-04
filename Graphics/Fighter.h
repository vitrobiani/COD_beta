#pragma once
#include "definitions.h"
#include "State.h"
#include "Soldier.h"
#include "StateSearchEnemy.h"

class Fighter: public Soldier 
{
private:
	int ammo, grenade_count;
	bool isReloading;
	int reloadTime;
	Bullet* bullet;
	Grenade* grenade;
public:
	Fighter(Position start_pos, TeamID tid);
	void engageEnemy();
	int getAmmo() { return ammo; };
	void setAmmo(int a) { ammo = a; };
	int getGrenadeCount() { return grenade_count; };
	void setGrenadeCount(int g) { grenade_count = g; };
	void setIsReloading(bool r) { isReloading = r; };
	void setBullet(Bullet* b) { bullet = b; };
	Bullet* getBullet() { return bullet; };
	void loadBullet();
	void loadGrenade();

};

