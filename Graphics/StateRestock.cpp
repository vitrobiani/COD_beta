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
	State* oldState = p->getState();
	p->setState(new StateFindTeammates());
	delete oldState;
	oldState = nullptr;
}

void StateRestock::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
