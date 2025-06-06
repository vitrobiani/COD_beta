#include "StateFindTeammates.h"

void StateFindTeammates::OnEnter(Soldier* p)
{
	Squire* s = (Squire*)(p);
	s->setIsMoving(true);
	Position teammate_pos = Team::findNearestTeammate(s);
	s->moveToTeammate(teammate_pos);
	if (Team::calculateDistance(s->getPos(), teammate_pos) < SQUIRE_DISTANCE_FROM_TEAMMATE)
		Transition(p);
}

void StateFindTeammates::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	p->setState(new StateTakeCover());
	delete oldState;
	oldState = nullptr;
}

void StateFindTeammates::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
