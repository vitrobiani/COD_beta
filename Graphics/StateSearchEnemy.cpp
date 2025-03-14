#include "StateSearchEnemy.h"

void StateSearchEnemy::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	f->setIsMoving(true);
	//f->setTarget(Team::findTarget(f));
	Position enemy_pos = Team::findNearestEnemy(f);
	f->moveToEnemy(enemy_pos);
}

void StateSearchEnemy::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	p->setState(new StateEngageEnemy());
	delete oldState;
	oldState = nullptr;
	p->getState()->OnEnter(p);
}

void StateSearchEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
