#include "StateRestock.h"
#include "StateFindTeammates.h"

void StateRestock::OnEnter(Soldier* p)
{
	Squire* s = (Squire*)(p);
	s->Restock();
}

void StateRestock::Transition(Soldier* p)
{
	OnExit(p);
	p->setState(new StateFindTeammates());
	p->getState()->OnEnter(p);
}

void StateRestock::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
