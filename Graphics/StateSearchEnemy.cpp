#include "StateSearchEnemy.h"

void StateSearchEnemy::OnEnter(Soldier* p)
{
	p->setIsMoving(true);
	Position enemy_pos = Team::findNearestEnemy(p);
	p->moveToEnemy(enemy_pos);
}

void StateSearchEnemy::Transition(Soldier* p)
{
	OnExit(p);
	p->setState(new StateEngageEnemy());
	p->getState()->OnEnter(p);
}

void StateSearchEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
