#include "StateProvideHelp.h"
#include "StateTakeCover.h"
#include "StateRestock.h"

void StateProvideHelp::OnEnter(Soldier* p)
{
	Squire* s = (Squire*)(p);
	Fighter* f = (Fighter*)(Team::callingSquires.front());
	int code = s->HelpSoldier(f);
	switch (code)
	{
	case TOO_FAR_TO_HELP:
		s->setIsMoving(true);
		s->moveToTeammate(f->getPos());
		break;
	case HELPED_SOLDIER:
		Team::callingSquires.pop();
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
	if (restock)
		p->setState(new StateRestock());
	else
		p->setState(new StateTakeCover());
	p->getState()->OnEnter(p);
}

void StateProvideHelp::OnExit(Soldier* p)
{
	p->setIsMoving(true);
}
