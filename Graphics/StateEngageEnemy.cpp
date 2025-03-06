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
	State* oldState = p->getState();
	if (f->getHP() < f->getHP_TH() || f->getAmmo() <= 0)
		f->setState(new StateCallSquire());
	else
		f->setState(new StateSearchEnemy());

	delete oldState;
	p->getState()->OnEnter(p);
}

void StateEngageEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(true);
}
