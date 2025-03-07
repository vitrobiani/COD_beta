#pragma once
#include "definitions.h"
#include "Soldier.h"
#include "Squire.h"
#include "Cell.h"

class Team
{
private:
	TeamID teamID;
	vector<Soldier*> soldiers{};
	vector<array<double, 3>> teamColor;
	int teamSize;
public:
	static vector<Team*> Teams;
	queue<Soldier*> callingSquires;

	Team(vector<array<double, 3>> tc);
	TeamID generateTeamID();
	TeamID generateTeamIDForSoldier();
	TeamID getTeamID() { return teamID; };
	vector<Soldier*>& getSoldiers() { return soldiers; };
	void setSoldiers(vector<Soldier*> s) { soldiers = s; };
	array<double, 3> getFighterColor() { return teamColor.at(0); };
	array<double, 3> getSquireColor() { return teamColor.at(1); };
	int getSize() { return teamSize; };
	static Position findNearestEnemy(Soldier* s);
	static Position findNearestTeammate(Soldier* s);
	static Soldier* findNearestTeammatePtr(Soldier* s);
	static double calculateDistance(Position p1, Position p2);
	void getEnemiesPositions(vector<Position>& positions);
	//vector<Position> getSoldiersPositions();
	static Team* getTeamByID(TeamID id);
	void addSoldier(Position start_pos, bool isFighter);

	// this return the next cell the soldier should go to to get to saftey
	static Position findSafestPosition(int maze[MSZ][MSZ], Soldier* s, double* sec_map);
	static Cell* findBestHeuristicCell(int maze[MSZ][MSZ], Soldier* s, double* sec_map);
	static Cell* BFSIteration(queue<Cell*>& grays, int maze[MSZ][MSZ]);
	static void CheckNeighbor(Position p, Cell* pCurrent, int maze[MSZ][MSZ], queue<Cell*>& grays);
	static Cell* RestorePath(Cell* pc);
};

