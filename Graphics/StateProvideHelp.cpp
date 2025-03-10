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
		Position targetPos = f->getPos();
		//s->stuckCheckDeque.push_back(targetPos);
		//if (s->stuckCheckDeque.size() > 2)
		//{
		//	int indexToCheck = s->stuckCheckDeque.size() - 3;	// potential reoccurance of the same position
		//	if (targetPos.row != s->stuckCheckDeque.at(indexToCheck).row || targetPos.col != s->stuckCheckDeque.at(indexToCheck).col)
		//	{
		//		s->stuckCheckDeque.pop_front();
		//	}
		//	else if (s->stuckCheckDeque.size() > soldierStucknessLimit)
		//	{
		//		int diffX = s->getPos().col - targetPos.col;
		//		int diffY = s->getPos().row - targetPos.row;
		//		int altNextMoveStatus = maze[s->getPos().row + diffY][s->getPos().col + diffX];	// look at the opposite of the best cell to move to
		//		if (altNextMoveStatus != WALL && altNextMoveStatus != AMMO_STASH && altNextMoveStatus != HP_STASH)
		//		{
		//			targetPos.col = s->getPos().col + diffX;
		//			targetPos.row = s->getPos().row + diffY;
		//		}
		//		s->stuckCheckDeque.clear();
		//	}
		//}
		s->moveToTeammate(targetPos);
		break;
	case HELPED_SOLDIER:
		Team::Teams.at(s->getID().team)->callingSquires.pop();
		break;
	case RESTOCK:
		restock = true;
		Transition(p);
		break;
	case IS_DEAD:
		Team::Teams.at(s->getID().team)->callingSquires.pop();
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
