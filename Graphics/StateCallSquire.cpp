#include "StateCallSquire.h"

void StateCallSquire::OnEnter(Soldier* p)
{
}

void StateCallSquire::Transition(Soldier* p)
{
	OnExit(p);
	p->setState(new StateSearchEnemy());
	p->getState()->OnEnter(p);
}

void StateCallSquire::OnExit(Soldier* p)
{
}
