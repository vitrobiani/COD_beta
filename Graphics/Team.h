#pragma once
#include "definitions.h"
#include "Soldier.h"
class Team
{
private:
	int teamID;
	vector<Soldier*> soldiers;
	int teamColor[3];
public:
	Team();
};

