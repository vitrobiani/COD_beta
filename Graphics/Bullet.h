#pragma once
#include "definitions.h"
#include "glut.h"
#include "Team.h"

class Bullet
{
private:
	double x, y;
	double dir_angle;
	double dirX, dirY;
	double speed;
	bool isMoving, isVirtual;
	TeamID id;
public:
	static vector<Bullet*> bullets;
	Bullet(double xx, double yy, double angle);
	Bullet(double xx, double yy, double angle, TeamID tid);
	Position move(int maze[MSZ][MSZ]);
	void show();
	void setIsMoving(bool value) { isMoving = value; }
	Position getPos() { return Position{ (int)x, (int)y }; }
	bool getIsMoving() { return isMoving; }	
	void simulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ]);
	bool findEnemyByExplosion(int maze[MSZ][MSZ], Position enemy_pos);
	bool moveTillEnemyOrWall(int maze[MSZ][MSZ], Position enemy_pos);
	Soldier* findSoldierAtPosition(Position pos);
	bool isEnemyTeam(Soldier* soldier);
};

