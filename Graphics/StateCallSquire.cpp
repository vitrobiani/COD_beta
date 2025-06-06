#include "StateCallSquire.h"

void StateCallSquire::OnEnter(Soldier* p)
{
	Fighter* f = (Fighter*)(p);
	f->move(Team::findSafestPosition(maze, f, security_maps.at(p->getID().team)));
	f->addToSquireQueue();
	f->checkIfFitForFight();
}

void StateCallSquire::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	p->setState(new StateSearchEnemy());
	delete oldState;
	oldState = nullptr;
}

void StateCallSquire::OnExit(Soldier* p)
{
}
