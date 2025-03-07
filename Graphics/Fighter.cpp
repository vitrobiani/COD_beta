#include "Fighter.h"

Fighter::Fighter(Position start_pos, TeamID tid) : Soldier(start_pos, tid)
{
	ammo = MAX_BULLET_CAPACITY_FIGHTER;
	grenade_count = MAX_GRENADE_CAPACITY_FIGHTER;
	state = new StateSearchEnemy();
	isReloading = false;
	reloadTime = 0;
	ammo_th = 2;
	isCallingSquire = false;
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
	if (hp < hp_th)
		state->Transition(this);

	if (isReloading)
	{
		reloadTime--;
		if (reloadTime <= 0) isReloading = false;
		return;
	}
	else
	{
		loadBullet(enemy_pos);
	}

	if (!isEnemyInSight(enemy_pos))
	{
		state->Transition(this);
	}
}

bool Fighter::isEnemyInSight(Position enemy_pos) 
{
	for (int i = 0; i < IS_ENEMY_IN_SIGHT; i++)
	{
		Grenade* g = new Grenade(getPos().row, getPos().col);

		if(g->findEnemyByExplosion(maze, enemy_pos))
			return true;
        else
			continue;
	}
	return false;
}

void Fighter::moveToEnemy(Position enemy_pos)
{
	bool isEnemyInSightBool = false;
	if (isMoving)
	{
		cloneMaze(maze, dupMaze);
		Cell* c = runAS(dupMaze, security_maps.at(getID().team), enemy_pos);
		move(Position{ c->getRow(), c->getCol() });
		isEnemyInSightBool = isEnemyInSight(enemy_pos);
	}
	if (isEnemyInSightBool)
		state->Transition(this);
}

void Fighter::addToSquireQueue() 
{
	if (!isCallingSquire) {
		isCallingSquire = true;
		Team::Teams.at(id.team)->callingSquires.push(this);
	}
}

void Fighter::checkIfFitForFight()
{
	if (ammo > ammo_th && hp > hp_th) {
		isCallingSquire = false;
		state->Transition(this);
	}
}

