#include "StateEngageEnemy.h"

void StateEngageEnemy::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	Position enemy_pos = Team::findNearestEnemy(f);
	f->engageEnemy(enemy_pos);
}

void StateEngageEnemy::Transition(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	OnExit(p);
	if (f->getHP() < f->getHP_TH() || f->getAmmo() <= 0)
		p->setState(new StateCallSquire());
	else
		f->setState(new StateSearchEnemy());

	p->getState()->OnEnter(p);
}

void StateEngageEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(true);
}
