#include "StateCallSquire.h"

void StateCallSquire::OnEnter(Soldier* p)
{
	Cell* c = Team::runBFS(maze, p, security_maps.at(p->getID().team));
	p->move(Position{ c->getRow(), c->getCol() });
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
