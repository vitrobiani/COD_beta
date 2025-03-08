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
	passive = (rand() % 2 == 0);
}

Fighter::~Fighter()
{
	delete state;
	state = nullptr;
}

void Fighter::defensiveManouver()
{
	if (isMoving)
	{
		cloneMaze(maze, dupMaze);
		Position c = Team::findSafestPosition(maze, this, security_maps.at(getID().team));
		move(c);
	}
}

void Fighter::loadGrenade(Position enemy_pos)
{
	if (grenade_count > 0 && Team::calculateDistance(getPos(), enemy_pos) <= GRENADE_THROWING_DISTANCE)
	{
		Grenade* g = new Grenade(getPos().row, getPos().col, getID());
		g->setIsExpending(true);
		g->explode();
		Grenade::grenades.push_back(g);
		grenade_count--;
	}
}

void Fighter::loadBullet(Position enemy_pos)
{
	if (ammo > ammo_th)
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
		if (reloadTime <= 0) {
			isReloading = false;
		}
	}
	
	if (hp < hp_th)
	{
		state->Transition(this);
		return;
	}

	if (!isEnemyInSight(enemy_pos))
	{
		state->Transition(this);
		return;
	}

	int action = rand() % 100;
	if (action > 50 && !isReloading)
	{
		loadBullet(enemy_pos);
	}
	else if (action <= 50 && action >= 25) {
		loadGrenade(enemy_pos);
	}
	else if (action < 25) {
		setIsMoving(true);
		defensiveManouver();
		setIsMoving(false);
	}
}

bool Fighter::isEnemyInSight(Position enemy_pos) 
{
	bool retVal = false;
	for (int i = 0; i < IS_ENEMY_IN_SIGHT; i++)
	{
		Grenade* g = new Grenade(getPos().row, getPos().col);

		if (g->findEnemyByExplosion(maze, enemy_pos))
		{
			retVal = true;
		}

		delete g;

		if (retVal)
		{
			break;
		}
	}
	return retVal;
}

bool Fighter::isEnemyInSameRoomAsMe(Position enemy_pos)
{
	return false;
}

void Fighter::moveToEnemy(Position enemy_pos)
{
	bool isEnemyInSightBool = false;
	if (isMoving)
	{
		cloneMaze(maze, dupMaze);
		Position c = runAS(dupMaze, security_maps.at(getID().team), enemy_pos);
		move(c);
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

