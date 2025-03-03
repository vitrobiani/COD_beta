#include "StateSearchEnemy.h"

void StateSearchEnemy::OnEnter(Soldier* p)
{
	p->setIsMoving(true);
	Position enemy = p->findNearestEnemy();
}

void StateSearchEnemy::Transition(Soldier* p)
{
}

void StateSearchEnemy::OnExit(Soldier* p)
{
}
