#pragma once
#include "definitions.h"
#include "Soldier.h"
#include "Fighter.h"
#include "Squire.h"
#include "Cell.h"

class Team
{
private:
	TeamID teamID;
	vector<Soldier*> soldiers;
	int teamColor[3];
	int teamSize;
public:
	static vector<Team*> Teams;
	Team(int color[3]);
	TeamID generateTeamID();
	TeamID generateTeamIDForSoldier();
	void addSoldier(Position start_pos);
	TeamID getTeamID() { return teamID; };
	vector<Soldier*> getSoldiers() { return soldiers; };
	void setSoldiers(vector<Soldier*> s) { soldiers = s; };
	int* getTeamColor() { return teamColor; };
	int getSize() { return teamSize; };
	static Position findNearestEnemy(Soldier* s);
	static double calculateDistance(Position p1, Position p2);
	vector<Position> getEnemiesPositions();
	vector<Position> getSoldiersPositions();
	static Team getTeamByID(TeamID id);

	// this return the next cell the soldier should go to to get to saftey
	static Cell* runBFS(int maze[MSZ][MSZ], Soldier* s, double* sec_map);
	static Cell* BFSIteration(queue<Cell*>& grays, int maze[MSZ][MSZ]);
	static void CheckNeighbor(Position p, Cell* pCurrent, int maze[MSZ][MSZ], queue<Cell*>& grays);
	static Cell* RestorePath(Cell* pc);
};

