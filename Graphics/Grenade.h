#pragma once
#include "Bullet.h"
#include "definitions.h"

const int NUM_BULLETS = 20;

class Grenade
{
private:
	double row, col;
	Bullet* bullets[NUM_BULLETS];
public:
	Grenade(double r, double c);
	void explode();
	void expand(int maze[MSZ][MSZ]);
	void show();
	void simulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ]);
};

