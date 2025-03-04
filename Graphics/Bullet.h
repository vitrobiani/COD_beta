#pragma once
#include "definitions.h"
#include "glut.h"

class Bullet
{
private:
	double x, y;
	double dir_angle;
	double dirX, dirY;
	double speed;
	bool isMoving;
	TeamID id;
public:
	Bullet(double xx, double yy, double angle);
	Bullet(double xx, double yy, double angle, TeamID tid);
	void move(int maze[MSZ][MSZ]);
	void show();
	void setIsMoving(bool value) { isMoving = value; }
	void simulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ]);
	bool findEnemyByExplosion(int maze[MSZ][MSZ], Position enemy_pos);
	bool moveTillEnemyOrWall(int maze[MSZ][MSZ], Position enemy_pos);
};

