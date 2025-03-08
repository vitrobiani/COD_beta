#include "Squire.h"
#include "Fighter.h"
#include "StateFindTeammates.h"

Squire::Squire(Position start_pos, TeamID tid) : Soldier(start_pos, tid)
{
	ammo = MAX_BULLET_CAPACITY_SQUIRE;
	grenade_count = MAX_GRENADE_CAPACITY_SQUIRE;
	health_pack = MAX_HEALTH_PACK_CAPACITY_SQUIRE;
	state = new StateFindTeammates();
	ammo_th = 2;
	health_pack_th = 2;
	prioritize_ammo = false;
}

Squire::~Squire()
{
	delete state;
	state = nullptr;
}

void Squire::moveToTeammate(Position teammate_pos)
{
	if (isMoving)
	{
		cloneMaze(maze, dupMaze);
		Position c = runAS(dupMaze, security_maps.at(getID().team), teammate_pos);
		move(c);
	}
}

void Squire::doesAnybodyNeedHelp()
{ 
	bool exitTakeCover = !Team::Teams.at(id.team)->callingSquires.empty() && (getState()->toString().compare((new StateTakeCover)->toString()) == 0 );
	bool exitProvideHelp = Team::Teams.at(id.team)->callingSquires.empty() && (getState()->toString().compare((new StateProvideHelp)->toString()) == 0);
	if(exitTakeCover || exitProvideHelp)
		this->getState()->Transition(this);
}

int Squire::HelpSoldier(Soldier* s)
{
	Fighter* f = (Fighter*)s;
	if (Team::calculateDistance(getPos(), f->getPos()) > SQUIRE_DISTANCE_FROM_TEAMMATE)
	{
		return TOO_FAR_TO_HELP;
	}

	if (f->getHP() < START_HP)
	{
		if (health_pack > 0)
		{
			f->setHP(START_HP);
			health_pack--;
		}
		else
		{
			return RESTOCK;
		}
	}

	if (f->getAmmo() < MAX_BULLET_CAPACITY_FIGHTER)
	{
		if (ammo > (MAX_BULLET_CAPACITY_FIGHTER - f->getAmmo()))
		{
			ammo -= (MAX_BULLET_CAPACITY_FIGHTER - f->getAmmo());
			f->setAmmo(MAX_BULLET_CAPACITY_FIGHTER);
		}
		else if (ammo > 0)
		{
			f->setAmmo(f->getAmmo() + ammo);
			ammo = 0;
			return RESTOCK;
		}
		else
		{
			return RESTOCK;
		}
	}
	return HELPED_SOLDIER;
}

void Squire::Restock()
{
	if (!prioritize_ammo && health_pack < health_pack_th && goToStash(HP_Stashes)) {
		health_pack = MAX_HEALTH_PACK_CAPACITY_SQUIRE;
		prioritize_ammo = !prioritize_ammo;
	}

	if (prioritize_ammo && ammo < ammo_th && goToStash(Ammo_Stashes)) {
		ammo = MAX_BULLET_CAPACITY_SQUIRE;
		prioritize_ammo = !prioritize_ammo;
	}

	if (ammo > ammo_th && health_pack > health_pack_th)
		getState()->Transition(this);
}

bool Squire::goToStash(const vector<Position>& stash)
{
	Position p = this->findNearestStash(stash);
	if (Team::calculateDistance(getPos(), p) > SQUIRE_DISTANCE_FROM_TEAMMATE)
	{
		cloneMaze(maze, dupMaze);
		Position c = this->runAS(dupMaze, security_maps.at(getID().team), p);
		move(c);
		return false;
	}
	else
	{
		return true;
	}
}

Position Squire::findNearestStash(const vector<Position>& stash)
{
	double distance = INFINITY;
	int nearest = 0;
	int index = 0;
	for (Position p : stash)
	{
		double dis = Team::calculateDistance(getPos(), p);
		if (dis < distance)
		{
			distance = dis;
			nearest = index;
		}
		index++;
	}
	return stash.at(nearest);
}