#include "Fighter.h"

Fighter::Fighter(Position start_pos, TeamID tid) : Soldier(start_pos, tid)
{
	ammo = MAX_BULLET_CAPACITY_FIGHTER;
	grenade_count = MAX_GRENADE_CAPACITY_FIGHTER;
	state = new StateSearchEnemy();
}
