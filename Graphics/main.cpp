#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include "definitions.h"
#include "Cell.h"
#include <iostream>
#include "Room.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Soldier.h"
#include "Fighter.h"

using namespace std;

const int WIDTH = 700;
const int HEIGHT = 700;

const int NUM_ROOMS = 14;
const int NUM_OBSTACLES = 700;

const double WALL_COST = 5;
const double SPACE_COST = 1;

Room* rooms[NUM_ROOMS];

bool bulletFired = false;
bool grenadeThrown = false;
Bullet* pb = nullptr;
Grenade* pg = nullptr;

int maze[MSZ][MSZ] = {0}; // WALLs
double security_map[MSZ][MSZ] = {0};	// mey mey
vector<double*> security_maps;

bool setGridlines = false;
bool showSecurityMap = false;

void RestorePath(Cell* pc)
{
	int row, col;
	while (pc != nullptr)
	{
		row = pc->getRow();
		col = pc->getCol();
		if (maze[row][col] == WALL)
		{
			maze[row][col] = SPACE;
		}
		pc = pc->getParent();
	}
}

// 'row' and 'col' are the indices of neighbor cell
// blacks shouldn't be changed
void AddNeighbor(int row, int col, Cell* pCurrent, priority_queue<Cell*, vector<Cell*>, CompareCells> &pq, vector<Cell> &grays, vector<Cell> blacks)
{
	double newg, cost;
	vector<Cell>::iterator itGray;
	vector<Cell>::iterator itBlack;

	if (maze[row][col] == WALL)
		cost = WALL_COST;
	else
		cost = SPACE_COST;

	newg = pCurrent->getG() + cost;
	Cell* pNeighbor = new Cell(row, col, pCurrent->getTargetRow(), pCurrent->getTargetCol(), newg, pCurrent, pCurrent->getSecurityMap());

	// check what to do with the neighbor Cell
	// 1.	if the neighbor is black: do nothing
	// 2.	if the neighbor is white: add it to PQ and to grays
	// 3.	if the neighbor is gray:
	// 3.1	if F of neighbor is not below the copy that is stored in PQ then do nothing
	// 3.2	otherwise, then we must update the PQ and grays
	itGray = find(grays.begin(), grays.end(), *pNeighbor);
	itBlack = find(blacks.begin(), blacks.end(), *pNeighbor);

	if (itBlack == blacks.end())	// then it is not black
	{
		if (itGray == grays.end())	// then it is not gray => it is white
		{
			// paint it gray 
			pq.push(pNeighbor);
			grays.push_back(*pNeighbor);
		}
		else  // it is gray
		{
			if (pNeighbor->getF() < itGray->getF())	// then we found a better path and we have to exchange it
			{
				grays.erase(itGray);
				grays.push_back(*pNeighbor);

				// and do the same with pq
				vector<Cell*> tmp;
				while (!pq.empty() && !(*pq.top() == *pNeighbor))
				{
					tmp.push_back(pq.top());	// save the top element in tmp
					pq.pop();	// remove top element from pq
				}
				if (pq.empty())	// Error
				{
					cout << "Error in AddNeighbor: pq is empty\n";
					exit(2);
				}
				else // we have found the neighbor cell in pq
				{
					pq.pop();	// remove old neighbor from pq
					pq.push(pNeighbor);
					// now we restore pq
					while (!tmp.empty())
					{
						pq.push(tmp.back());
						tmp.pop_back();
					}
				}
			}
		}
	}
}

// run A* from room at index1 to room at index2
void BuildPath(int index1, int index2)
{
	int row, col, target_row, target_col;
	row = rooms[index1]->getCenterY();
	col = rooms[index1]->getCenterX();
	target_row = rooms[index2]->getCenterY();
	target_col = rooms[index2]->getCenterX();

	Cell* pCurrent;
	Cell* start = new Cell(row, col, target_row, target_col, 0, nullptr, nullptr);

	priority_queue<Cell*, vector<Cell*>, CompareCells> pq;
	vector<Cell> grays;
	vector<Cell> blacks;
	vector<Cell>::iterator itGray;

	pq.push(start);
	grays.push_back(*start);

	// pq shouldn't be empty because we are going to reach the target beforehand
	while (!pq.empty())	
	{
		pCurrent = pq.top();
		if (pCurrent->getH() < 0.001)	// this is a target cell
		{
			RestorePath(pCurrent);
			return;
		}
		else // target hasn't been reached
		{
			// 1. remove pCurrent from pq
			pq.pop();

			// 2. find and remove pCurrent from grays
			itGray = find(grays.begin(), grays.end(), *pCurrent);
			if (itGray == grays.end())	// pCurrent wasn't found
			{
				cout << "Error in building path: pCurrent wasn't found in grays\n";
				exit(1);
			}
			grays.erase(itGray);

			// 3. paint pCurrent black
			blacks.push_back(*pCurrent);

			// 4. take care of neighbors
			row = pCurrent->getRow();
			col = pCurrent->getCol();
			// up
			if (row + 1 < MSZ)
			{
				AddNeighbor(row +1, col, pCurrent, pq, grays, blacks);
			}
			// down
			if (row - 1 >= 0)
			{
				AddNeighbor(row - 1, col, pCurrent, pq, grays, blacks);
			}
			// left
			if (col - 1 >= 0)
			{
				AddNeighbor(row, col - 1, pCurrent, pq, grays, blacks);
			}
			// right
			if (col + 1 < MSZ)
			{
				AddNeighbor(row, col + 1, pCurrent, pq, grays, blacks);
			}
		}
	}
	cout << "Error in building path: pq is empty, target hasn't been found\n";
}

void BuildPathBetweenRooms()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		for (j = i + 1; j < NUM_ROOMS; j++)
		{
			BuildPath(i, j);	// A*
			cout << "The path from " << i << " to " << j << " has been built\n";
		}
	}
}

void initSoldierTeams()
{
	for (int i = 0; i < TEAM_NUM; i++)
	{
		int r = rand() % NUM_ROOMS;
		Room* room = rooms[r];
		for (int j = 0; j < TEAM_SIZE; j++) {
			int row = room->getCenterY() - room->getHeight() / 2 + (rand() % room->getHeight());
			int col = room->getCenterX() - room->getWidth() / 2 + (rand() % room->getWidth());
			Position p = { row , col};
			TeamID t = { i, j };
			Fighter* s = new Fighter(p, t);
			Soldier::addSoldier(s);
			cout << "Soldier " << s->getPos().col << " " << s->getPos().row << " added\n";
		}
	}
}

void SetupDungeon()
{
	int i,j;
	int cx, cy, w, h;
	bool hasOverlap;

	for (i = 0;i < NUM_ROOMS;i++)
	{
		do
		{
			hasOverlap = false;
			w = 6 + rand() % (MSZ / 5);
			h = 6 + rand() % (MSZ / 5);
			cx = 2 + w / 2 + rand() % (MSZ - w - 6);
			cy = 2 + h / 2 + rand() % (MSZ - h - 6);
			for(j=0;j<i && !hasOverlap;j++)
				hasOverlap = rooms[j]->Overlap(cx, cy, w, h);
		} while (hasOverlap); // check the validity of the room
			
		rooms[i] = new Room(cx, cy, w, h,maze);
	}

	for (i = 0; i < NUM_OBSTACLES; i++)
	{
		maze[rand() % MSZ][rand() % MSZ] = WALL;
	}

	BuildPathBetweenRooms();

	initSoldierTeams();
	int k, l;
	for (k = 0; k < Soldier::Teams.size(); k++)
	{
		for (l = 0; l < Soldier::Teams.at(k).size(); l++)
		{
			Position p = Soldier::Teams.at(k).at(l)->getPos();
			cout << "Soldier " << p.col << " " << p.row << " added\n";
			maze[p.row][p.col] = SOLDIER;
		}
	}

}

void ShowDungeon()
{
	int i, j;
	double s;

	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			if (showSecurityMap) s = security_map[i][j];
			else s = 0;
			//1. set color of cell
			switch (maze[i][j])
			{
			case SPACE:
				glColor3d(1 - s, 1 - s, 1 - s); // white to gray
				break;
			case WALL:
				glColor3d(0.3, 0.3, 0.4); // dark gray
				break;
			case SOLDIER:
				glColor3d(1, 0, 0); // red
				break;
			}
			// show cell
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j + 1, i + 1);
			glVertex2d(j + 1, i);
			glEnd();

			// set grid lines
			if (setGridlines) {
				glColor3d(0, 0, 0);
				glBegin(GL_LINE_LOOP); 
				glVertex2d(j, i);
				glVertex2d(j, i + 1);
				glVertex2d(j + 1, i + 1);
				glVertex2d(j + 1, i);
				glEnd();
			}
		}
	}
}

void GenerateSecurityMap()
{
	for (int j = 0; j < SEC_MAP_ITERATIONS; j++)
	{
		int numSimulations = GRENADE_SEC_MAP_AMOUNT;
		int i;

		for (i = 0; i < numSimulations; i++)
		{
			Grenade* g = new Grenade(rand() % MSZ, rand() % MSZ);

			g->simulateExplosion(maze, security_map);
		}
	}
}

void GenerateSecurityMapForSpsificTeam(int TeamNum)
{
	for (int i = 0; i < Soldier::Teams.size() ; i++)
	{
		if (i == TeamNum) continue;
		for (Soldier* s: Soldier::Teams.at(i))
		{
			for (int j = 0; j < GRENADE_SEC_MAP_PER_TEAM; j++)
			{
				Grenade* g = new Grenade(s->getPos().row, s->getPos().col);
				
				double clonedMap[MSZ][MSZ] = { 0 };
				cloneSecurityMap(security_map, clonedMap);
				g->simulateExplosion(maze, clonedMap);
				cloneSecurityMapToPtr(clonedMap, security_maps.at(s->getID().team));
			}
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	ShowDungeon();

	if (pb != nullptr)
	{
		pb->show();
	}
	if (pg != nullptr)
	{
		pg->show();
	}
	for (int i = 0; i < Soldier::Teams.size(); i++)
		for (Soldier* s : Soldier::Teams.at(i)) {
			Fighter* f = (Fighter*)(s);
			if (f->getBullet())
				f->getBullet()->show();
		}

	glutSwapBuffers(); // show all
}

void initSecurityMaps() {
	for (int i = 0; i < TEAM_NUM; i++) {
		double* sec_map = new double[MSZ * MSZ];
		security_maps.push_back(sec_map);
	}
}

void init()
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	SetupDungeon();
	
	GenerateSecurityMap();
	initSecurityMaps();
}

void cloneAllSecMaps() {
	for (int i = 0; i < security_maps.size(); i++) {
		for (int j = 0; j < MSZ; j++) {
			for (int k = 0; k < MSZ; k++) {
				security_maps.at(i)[MSZ * j + k] = security_map[j][k];
			}
		}
	}
}

void idle() 
{
	Sleep(50);
	if (bulletFired)
	{
		pb->move(maze);
	}
	if (grenadeThrown)
	{
		pg->expand(maze);
	}

	cloneAllSecMaps();
	for (int i = 0; i < security_maps.size(); i++)
		GenerateSecurityMapForSpsificTeam(i);

	for (int i = 0; i < Soldier::Teams.size(); i++)
	{
		for (Soldier* s : Soldier::Teams.at(i))
		{
			s->getState()->OnEnter(s);
		}
	}

	glutPostRedisplay(); // indirect call to display
}

void menu(int choice)
{
	switch (choice)
	{
	case 1:	// fire bullet
		bulletFired = true;
		pb->setIsMoving(true);
		break;
	case 2:	// throw grenade
		grenadeThrown = true;
		pg->explode();
		break;
	case 3:	// security map
		GenerateSecurityMap();
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//pb = new Bullet(MSZ * x / (double)WIDTH, MSZ * (HEIGHT - y) / (double)HEIGHT, (rand() % 360) * PI / 180);
		pg = new Grenade(MSZ * (HEIGHT - y) / (double)HEIGHT, MSZ * x / (double)WIDTH);
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'r') {
		SetupDungeon();
	}
	if (key == 'g')
		if (setGridlines) setGridlines = false;
		else setGridlines = true;
	if (key == 's')
		if (showSecurityMap) showSecurityMap = false;
		else showSecurityMap = true;

}



void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	// definitions for visual memory (Frame buffer) and double buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(350, 70);
	glutCreateWindow("BFS");

	// display is a refresh function
	glutDisplayFunc(display);
	// idle is a update function
	glutIdleFunc(idle);

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Fire Bullet", 1);
	glutAddMenuEntry("Throw Grenade", 2);
	glutAddMenuEntry("Generate Security Map", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}