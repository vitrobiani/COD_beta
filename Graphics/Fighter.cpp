#include "Fighter.h"

Fighter::Fighter(Position start_pos, TeamID tid) : Soldier(start_pos, tid)
{
	ammo = MAX_BULLET_CAPACITY_FIGHTER;
	grenade_count = MAX_GRENADE_CAPACITY_FIGHTER;
	state = new StateSearchEnemy();
	isReloading = false;
	reloadTime = 0;
	bullet = nullptr;
	grenade = nullptr;
}

void Fighter::loadBullet()
{
	if (ammo > 0)
	{
		Position enemy_pos = findNearestEnemy();
		double angle = atan2(enemy_pos.row - pos.row, enemy_pos.col - pos.col);
		Bullet* b = new Bullet(pos.col, pos.row, angle, id);
		setBullet(b);
		b->setIsMoving(true);
		setIsReloading(true);
		reloadTime = RELOAD_TIME_FIGHTER;
		ammo--;
	}
	else {
		state->Transition(this);
	}
}

void Fighter::engageEnemy()
{
	if (isReloading)
	{
		reloadTime--;
		bullet->move(maze);
		if (reloadTime <= 0) isReloading = false;
		return;
	}
	loadBullet();
}
