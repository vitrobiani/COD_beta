#include "StateEngageEnemy.h"

void StateEngageEnemy::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	//Position enemy_pos = Team::findNearestEnemy(f);
	f->engageEnemy();
}

void StateEngageEnemy::Transition(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	OnExit(p);
	State* oldState = p->getState();
	if (f->getHP() < f->getHP_TH() || f->getAmmo() <= f->getAmmoTh())
		f->setState(new StateCallSquire());
	else
		f->setState(new StateSearchEnemy());

	delete oldState;
	oldState = nullptr;
	//p->getState()->OnEnter(p);
}

void StateEngageEnemy::OnExit(Soldier* p)
{
	p->setIsMoving(true);
}
