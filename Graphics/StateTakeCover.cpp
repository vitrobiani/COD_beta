#include "StateTakeCover.h"

void StateTakeCover::OnEnter(Soldier* p)
{
	Squire* s = (Squire*)(p);
	Soldier* teammate = Team::findNearestTeammatePtr(s);
	s->setIsMoving(true);
	
	int dupMaze[MSZ][MSZ] = { 0 };
	cloneMaze(maze, dupMaze);
	Cell* c = Team::findBestHeuristicCell(dupMaze, teammate, security_maps.at(p->getID().team));
	Position p1 = Position{ c->getRow(), c->getCol() };

	cloneMaze(maze, dupMaze);
	Cell* cell = s->runAS(dupMaze, security_maps.at(p->getID().team), p1);
	s->move(Position{cell->getRow(), cell->getCol()});
	s->doesAnybodyNeedHelp();
}

void StateTakeCover::Transition(Soldier* p)
{
	OnExit(p);
	State* oldState = p->getState();
	p->setState(new StateProvideHelp());
	delete oldState;
	p->getState()->OnEnter(p);
}

void StateTakeCover::OnExit(Soldier* p)
{
	p->setIsMoving(false);
}
