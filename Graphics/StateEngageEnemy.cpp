#include "StateEngageEnemy.h"

void StateEngageEnemy::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	f->engageEnemy();
}

void StateEngageEnemy::Transition(Soldier* p)
{
	OnExit(p);
	p->setState(new StateCallSquire());
	p->getState()->OnEnter(p);
}

void StateEngageEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(true);
}
