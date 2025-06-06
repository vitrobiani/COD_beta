#pragma once
#include "memory"
#include "Bullet.h"
#include "definitions.h"

const int NUM_BULLETS = 20;

class Grenade
{
private:
	double row, col;
	//Bullet* bullets[NUM_BULLETS];
	//unique_ptr<Bullet> bullets[NUM_BULLETS];
	vector<Bullet*> bullets;
	bool isExpending;
	TeamID id;
public:
	static vector<Grenade*> grenades;
	Grenade(double r, double c);
	Grenade(double r, double c, TeamID tid);
	~Grenade();
	void explode();
	void expand(int maze[MSZ][MSZ]);
	void show();
	void simulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ]);
	bool findEnemyByExplosion(int maze[MSZ][MSZ], Position enemy_pos);
	void setIsExpending(bool value) { isExpending = value; };
	bool getIsExpending() { return isExpending; };
};

