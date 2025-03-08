#pragma once
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <array>
#include <string>

using namespace std;

constexpr int MSZ = 100;

constexpr int TEAM_NUM = 2;
constexpr int TEAM_SIZE = 3;

constexpr int WIDTH = 700;
constexpr int HEIGHT = 700;

constexpr int NUM_ROOMS = 14;
constexpr int NUM_OBSTACLES = 700;

constexpr double WALL_COST = 5;
constexpr double SPACE_COST = 1;

const unordered_map<string, array<double, 3>> team_colors = {
    {"FPurple", {217 / (double)255, 1 / (double)255, 122 / (double)255}},
    {"SPurple", {219 / (double)255, 62/ (double)255, 177 / (double)255}},
    {"FOrange", {252 / (double)255, 76 / (double)255, 2 / (double)255}},
    {"SOrange", {255 / (double)255, 153 / (double)255, 19 / (double)255}}
};

constexpr int HP_STASH = 6;
constexpr int AMMO_STASH = 5;
constexpr int BLACK = 4;
constexpr int GRAY = 3;
constexpr int SOLDIER = 2;
constexpr int SPACE = 1;
constexpr int WALL = 0;
constexpr double PI = 3.1415;

constexpr int AMMO_STASH_AMOUNT = 2;
constexpr int HEALTH_PACK_STASH_AMOUNT = 2;

constexpr double SECURITY_FACTOR = 0.001;
constexpr double SECURITY_COEFFICIENT = 15;
constexpr int GRENADE_SEC_MAP_PER_TEAM = 30;
constexpr int IS_ENEMY_IN_SIGHT = 10;
constexpr double BULLET_SPEED = 0.3;
constexpr int BULLET_DAMAGE = 1;
constexpr int SQUIRE_DISTANCE_FROM_TEAMMATE = 5;
constexpr int GRENADE_THROWING_DISTANCE = 8;

constexpr int GRENADE_SEC_MAP_AMOUNT = 1000;
constexpr int SEC_MAP_ITERATIONS = 20;

constexpr int START_HP = 100;

constexpr int MAX_GRENADE_CAPACITY_SQUIRE = 20;
constexpr int MAX_BULLET_CAPACITY_SQUIRE = 200;
constexpr int MAX_BULLET_CAPACITY_FIGHTER = 12;
constexpr int MAX_GRENADE_CAPACITY_FIGHTER = 8;
constexpr int RELOAD_TIME_FIGHTER = 15;

constexpr int BFS_DEPTH = 1000;

constexpr int MAX_HEALTH_PACK_CAPACITY_SQUIRE = 12;

// Squire code definitions
constexpr int TOO_FAR_TO_HELP = 1;
constexpr int RESTOCK = 2;
constexpr int HELPED_SOLDIER = 3;

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
