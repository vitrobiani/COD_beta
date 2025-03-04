#include "StateSearchEnemy.h"

void StateSearchEnemy::OnEnter(Soldier* p)
{
	p->setIsMoving(true);
	p->move();
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
