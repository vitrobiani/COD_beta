#include "Bullet.h"
#include <math.h>

// angle in radians
Bullet::Bullet(double xx, double yy, double angle)
{
	x = xx;
	y = yy;
	dir_angle = angle;
	dirX = cos(angle);
	dirY = sin(angle);
	speed = 0.3;
	isMoving = false;
}

void Bullet::move(int maze[MSZ][MSZ])
{
	if (isMoving)
	{
		x += speed * dirX;
		y += speed * dirY;
		if (maze[(int)y][(int)x] == WALL)
		{
			isMoving = false;
		}
	}
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
