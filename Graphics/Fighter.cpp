#include "Fighter.h"

Fighter::Fighter(Position start_pos, TeamID tid) : Soldier(start_pos, tid)
{
	ammo = MAX_BULLET_CAPACITY_FIGHTER;
	grenade_count = MAX_GRENADE_CAPACITY_FIGHTER;
	state = new StateSearchEnemy();
	isReloading = false;
	reloadTime = 0;
}

void Fighter::loadBullet(Position enemy_pos)
{
	if (ammo > 0)
	{
		double angle = atan2(enemy_pos.row - pos.row, enemy_pos.col - pos.col);
		Bullet* b = new Bullet(pos.col+0.5, pos.row+0.5, angle, id);
		Bullet::bullets.push_back(b);
		b->setIsMoving(true);
		setIsReloading(true);
		reloadTime = RELOAD_TIME_FIGHTER;
		ammo--;
	}
	else {
		state->Transition(this);
	}
}

void Fighter::engageEnemy(Position enemy_pos)
{
	if (isReloading)
	{
		reloadTime--;
		if (reloadTime <= 0) isReloading = false;
		return;
	}
	loadBullet(enemy_pos);
}
