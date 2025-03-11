#include "StateSearchEnemy.h"

void StateSearchEnemy::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	f->setIsMoving(true);
	auto it = find(Team::dead_soldiers.begin(), Team::dead_soldiers.end(), f->getTarget());
	if (f->getTarget() == nullptr || it != Team::dead_soldiers.end())
	{
		if (f->getIsCooprative())
		{
			for (auto* s : Team::getTeamByID(f->getID())->getSoldiers()) {
				if (s->getID().place == f->getID().place)
					continue;
				if (!strcmp(s->getType(), "Fighter"))
				{
					Fighter* ftr = (Fighter*)s;
					if (ftr->getTarget() == nullptr || !ftr->getTarget()->getIsAlive()) {
						continue;
					}
					f->setTarget(ftr->getTarget());
					break;
				}
			}
			if (f->getTarget() == nullptr || !f->getTarget()->getIsAlive())
			{
				f->setTarget(Team::findTarget(f));
			}
		}
		else {
			f->setTarget(Team::findTarget(f));
		}
	}
	//Position enemy_pos = Team::findNearestEnemy(f);
	f->moveToEnemy();
}

void StateSearchEnemy::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	p->setState(new StateEngageEnemy());
	delete oldState;
	oldState = nullptr;
	//p->getState()->OnEnter(p);
}

void StateSearchEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
