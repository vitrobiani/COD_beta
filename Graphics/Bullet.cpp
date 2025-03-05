#include "Bullet.h"
#include <math.h>


vector<Bullet*> Bullet::bullets;
// angle in radians
Bullet::Bullet(double xx, double yy, double angle)
{
	x = xx;
	y = yy;
	dir_angle = angle;
	dirX = cos(angle);
	dirY = sin(angle);
	speed = BULLET_SPEED;
	isMoving = false;
}

Bullet::Bullet(double xx, double yy, double angle, TeamID tid)
{
	x = xx;
	y = yy;
	dir_angle = angle;
	dirX = cos(angle);
	dirY = sin(angle);
	speed = BULLET_SPEED;
	isMoving = false;
	id = tid;
}

Position Bullet::move(int maze[MSZ][MSZ])
{
	if (isMoving)
	{
		x += speed * dirX;
		y += speed * dirY;
		if (maze[(int)y][(int)x] == WALL)
		{
			isMoving = false;
		}
		if (maze[(int)y][(int)x] == SOLDIER)
		{
			isMoving = false;
			return Position{ (int)x, (int)y };
		}
	}
	return Position{ -1,-1 };
}

void Bullet::show()
{
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x - 0.5, y);
	glVertex2d(x, y + 0.5);
	glVertex2d(x + 0.5, y);
	glVertex2d(x, y - 0.5);
	glEnd();
}

void Bullet::simulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ])
{
	isMoving = true;
	while (isMoving)
	{
		security_map[(int)y][(int)x] += SECURITY_FACTOR;
		move(maze);
	}
}

bool Bullet::moveTillEnemyOrWall(int maze[MSZ][MSZ], Position enemy_pos)
{
	if (isMoving)
	{
		x += speed * dirX;
		y += speed * dirY;
		if (maze[(int)y][(int)x] == WALL)
		{
			isMoving = false;
			return false;
		}
		if ((int)x == enemy_pos.col && (int)y == enemy_pos.row)
		{
			isMoving = false;
			return true;
		}
		return false;
	}
	return false;
}

bool Bullet::findEnemyByExplosion(int maze[MSZ][MSZ], Position enemy_pos)
{
	isMoving = true;
	while (isMoving)
	{
		if (moveTillEnemyOrWall(maze, enemy_pos))
			return true;
		else
			continue;
	}
	return false;
}
