#pragma once
#include "definitions.h"
#include "State.h"
#include "CompareCells.h"
#include "Cell.h"
#include "Grenade.h"

class State;


class Soldier
{
protected:
	int hp;
	bool isMoving;
	Position pos;
	TeamID id;
	State* state;
public:
	//static vector<vector<Soldier*>> Teams;
	////static Soldier* Teams[TEAM_NUM][TEAM_SIZE];
	//static void addSoldier(Soldier* s) { Teams[s->getID().team].push_back(s); };

	Soldier(Position start_pos, TeamID tid);
	bool getIsMoving() { return isMoving; };
	void setIsMoving(bool b) { isMoving = b; };
	int getHP() { return hp; };
	void setHP(int h) { hp = h; };
	Position getPos() { return pos; };
	void setPos(Position p) { pos = p; };
	State* getState() { return state; };
	void setState(State* s) { state = s; };
	TeamID getID() { return id; };
	void moveToEnemy(Position enemy_pos);
	void move(Position p);
	bool checkHitByBullet();

    virtual const char* getType() = 0;
    virtual int getAmmo() = 0;
    virtual int getGrenades() = 0;

	Cell* runAS(int maze[MSZ][MSZ], double* security_map, Position target);
	Cell* ASIteration(priority_queue<Cell*, vector<Cell*>, CompareCells>& grays, int maze[MSZ][MSZ], Position target);
	Cell* CheckNeighbor(int row, int col, Cell* pCurrent, Position target, int maze[MSZ][MSZ], priority_queue<Cell*, vector<Cell*>, CompareCells>& grays);
	Cell* RestorePath(Cell * pc);

	bool isEnemyInSight(Position enemy_pos);
	//Cell* runBFS(int maze[MSZ][MSZ], int curRow, int curCol);
	//Cell* BFSIteration(queue<Cell*>& grays, int maze[MSZ][MSZ], vector<int> targets, bool forH);
};

