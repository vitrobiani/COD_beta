#include "Squire.h"

Squire::Squire(Position start_pos, TeamID tid) : Soldier(start_pos, tid)
{
	ammo = MAX_BULLET_CAPACITY_SQUIRE;
	grenade_count = MAX_GRENADE_CAPACITY_SQUIRE;
	health_pack = MAX_HEALTH_PACK_CAPACITY_SQUIRE;
}
