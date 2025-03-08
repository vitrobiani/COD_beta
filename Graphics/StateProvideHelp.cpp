#include "StateProvideHelp.h"
#include "StateTakeCover.h"
#include "StateRestock.h"

void StateProvideHelp::OnEnter(Soldier* p)
{
	Squire* s = (Squire*)(p);
	Fighter* f = (Fighter*)(Team::Teams.at(s->getID().team)->callingSquires.front());
	int code = s->HelpSoldier(f);
	switch (code)
	{
	case TOO_FAR_TO_HELP:
		s->setIsMoving(true);
		s->moveToTeammate(f->getPos());
		break;
	case HELPED_SOLDIER:
		Team::Teams.at(s->getID().team)->callingSquires.pop();
		break;
	case RESTOCK:
		restock = true;
		Transition(p);
		break;
	default:
		break;
	}
	s->doesAnybodyNeedHelp();
}

void StateProvideHelp::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	if (restock)
		p->setState(new StateRestock());
	else
		p->setState(new StateTakeCover());
	delete oldState;
	oldState = nullptr;
	p->getState()->OnEnter(p);
}

void StateProvideHelp::OnExit(Soldier* p)
{
	p->setIsMoving(true);
}
