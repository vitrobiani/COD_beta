#include "Grenade.h"

Grenade::Grenade(double r, double c)
{
	int i;
	row = r;
	col = c;
	double alpha = 2 * PI / NUM_BULLETS;	// bullet seperation angle
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i] = new Bullet(col, row, i * alpha);
	}
}

Grenade::Grenade(double r, double c, TeamID tid)
{
	int i;
	row = r;
	col = c;
	double alpha = 2 * PI / NUM_BULLETS;	// bullet seperation angle
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i] = new Bullet(col, row, i * alpha, tid);
	}
	id = tid;
}

void Grenade::explode()
{
	int i;
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i]->setIsMoving(true);
	}
}

void Grenade::expand(int maze[MSZ][MSZ])
{
	int i;
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i]->move(maze);
	}
}

void Grenade::show()
{
	int i;
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i]->show();
	}
}

void Grenade::simulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ])
{
	int i;

	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i]->simulateExplosion(maze, security_map);
	}
}

bool Grenade::findEnemyByExplosion(int maze[MSZ][MSZ], Position enemy_pos)
{
	int i;

	for (i = 0; i < NUM_BULLETS; i++)
	{
		if (bullets[i]->findEnemyByExplosion(maze, enemy_pos))
			return true;
		else
			continue;
	}
	return false;
}
