#pragma once
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

const int MSZ = 100; 

const int TEAM_NUM = 2;
const int TEAM_SIZE = 3;
const vector<int>;

const int BLACK = 4;
const int GRAY = 3;
const int SOLDIER = 2;
const int SPACE = 1;
const int WALL = 0;
const double PI = 3.1415;
const double SECURITY_FACTOR = 0.001;
const double SECURITY_COEFFICIENT = 15;
const int GRENADE_SEC_MAP_PER_TEAM = 30;
const int IS_ENEMY_IN_SIGHT = 4;

const int GRENADE_SEC_MAP_AMOUNT = 1000;
const int SEC_MAP_ITERATIONS = 20;

const int START_HP = 100;

const int MAX_GRENADE_CAPACITY_SQUIRE = 20;
const int MAX_BULLET_CAPACITY_SQUIRE = 200;
const int MAX_BULLET_CAPACITY_FIGHTER = 100;
const int MAX_GRENADE_CAPACITY_FIGHTER = 5;
const int RELOAD_TIME_FIGHTER = 15;

const int BFS_DEPTH = 100;

const int MAX_HEALTH_PACK_CAPACITY_SQUIRE = 20;

extern int maze[MSZ][MSZ];
extern double security_map[MSZ][MSZ];
extern vector<double*> security_maps;

typedef struct 
{
	int row;
	int col;
}Position;

typedef struct
{
	int team, place;
}TeamID;

void cloneMaze(int source[MSZ][MSZ], int target[MSZ][MSZ]);
void cloneSecurityMapToPtr(double source[MSZ][MSZ], double* target);
void cloneSecurityMap(double source[MSZ][MSZ], double target[MSZ][MSZ]);
