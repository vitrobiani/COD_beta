#pragma once
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <array>
#include <string>

using namespace std;

const int MSZ = 100; 

const int TEAM_NUM = 2;
const int TEAM_SIZE = 3;

const unordered_map<string, array<double, 3>> team_colors = {
    {"FPurple", {217 / (double)255, 1 / (double)255, 122 / (double)255}},
    {"SPurple", {219 / (double)255, 62/ (double)255, 177 / (double)255}},
    {"FOrange", {252 / (double)255, 76 / (double)255, 2 / (double)255}},
    {"SOrange", {255 / (double)255, 153 / (double)255, 19 / (double)255}}
};

const int HP_STASH = 6;
const int AMMO_STASH = 5;
const int BLACK = 4;
const int GRAY = 3;
const int SOLDIER = 2;
const int SPACE = 1;
const int WALL = 0;
const double PI = 3.1415;

const int AMMO_STASH_AMOUNT = 2;
const int HEALTH_PACK_STASH_AMOUNT = 2;

const double SECURITY_FACTOR = 0.001;
const double SECURITY_COEFFICIENT = 15;
const int GRENADE_SEC_MAP_PER_TEAM = 30;
const int IS_ENEMY_IN_SIGHT = 4;
const double BULLET_SPEED = 0.3;
const int BULLET_DAMAGE = 1;
const int SQUIRE_DISTANCE_FROM_TEAMMATE = 5;

const int GRENADE_SEC_MAP_AMOUNT = 1000;
const int SEC_MAP_ITERATIONS = 20;

const int START_HP = 100;

const int MAX_GRENADE_CAPACITY_SQUIRE = 20;
const int MAX_BULLET_CAPACITY_SQUIRE = 200;
const int MAX_BULLET_CAPACITY_FIGHTER = 10;
const int MAX_GRENADE_CAPACITY_FIGHTER = 5;
const int RELOAD_TIME_FIGHTER = 15;

const int BFS_DEPTH = 400;

const int MAX_HEALTH_PACK_CAPACITY_SQUIRE = 20;

// Squire code definitions
const int TOO_FAR_TO_HELP = 1;
const int RESTOCK = 2;
const int HELPED_SOLDIER = 3;

extern int maze[MSZ][MSZ];
extern int dupMaze[MSZ][MSZ];
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

extern vector<Position> HP_Stashes;
extern vector<Position> Ammo_Stashes;
void cloneMaze(int source[MSZ][MSZ], int target[MSZ][MSZ]);
void cloneSecurityMapToPtr(double source[MSZ][MSZ], double* target);
void cloneSecurityMap(double source[MSZ][MSZ], double target[MSZ][MSZ]);
