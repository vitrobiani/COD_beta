#include "StateEngageEnemy.h"

void StateEngageEnemy::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	Position enemy_pos = Team::findNearestEnemy(f);
	f->engageEnemy(enemy_pos);
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
