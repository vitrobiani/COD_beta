#include "StateCallSquire.h"

void StateCallSquire::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	f->move(Team::findSafestPosition(maze, p, security_maps.at(p->getID().team)));
	f->addToSquireQueue();
	f->checkIfFitForFight();
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
