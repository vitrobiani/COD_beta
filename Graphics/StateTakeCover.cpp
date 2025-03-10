#include "StateTakeCover.h"

void StateTakeCover::OnEnter(Soldier* p)
{
	Squire* s = (Squire*)(p);
	Soldier* teammate = Team::findNearestTeammatePtr(s);
	s->setIsMoving(true);
	
	cloneMaze(maze, dupMaze);
	Position c = Team::findBestHeuristicCell(dupMaze, teammate, security_maps.at(p->getID().team));

	cloneMaze(maze, dupMaze);
	Position pos = s->runAS(dupMaze, security_maps.at(p->getID().team), c);
	s->move(pos);
	s->doesAnybodyNeedHelp();
}

void StateTakeCover::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	p->setState(new StateProvideHelp());
	delete oldState;
	oldState = nullptr;
	//p->getState()->OnEnter(p);
}

void StateTakeCover::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
