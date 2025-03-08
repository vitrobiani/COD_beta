#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include <sstream>
#include "definitions.h"
#include "Cell.h"
#include <iostream>
#include "Room.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Soldier.h"
#include "Fighter.h"
#include "Team.h"
#include "Windows.h"

using namespace std;

Room* rooms[NUM_ROOMS] = { 0 };
vector<Room*> rooms_for_summoning;

bool bulletFired = false;
bool grenadeThrown = false;
Bullet* pb = nullptr;
Grenade* pg = nullptr;

int maze[MSZ][MSZ] = {0}; // WALLs
int dupMaze[MSZ][MSZ] = { 0 };
double security_map[MSZ][MSZ] = {0};	// mey mey
vector<double*> security_maps;
vector<Position> HP_Stashes;
vector<Position> Ammo_Stashes;

bool setGridlines = false;
bool showSecurityMap = false;
bool paused = true;

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

void initAmmoAndHealthPackStashes() {
	for (int i = 0; i < AMMO_STASH_AMOUNT; i++)
	{
		int r = rand() % rooms_for_summoning.size();
		Room* room = rooms_for_summoning.at(r);
		rooms_for_summoning.erase(rooms_for_summoning.begin() + r);
		int row = room->getCenterY() - room->getHeight() / 2 + (rand() % room->getHeight());
		int col = room->getCenterX() - room->getWidth() / 2 + (rand() % room->getWidth());
		maze[row][col] = AMMO_STASH;
		Ammo_Stashes.push_back(Position{ row, col });
	}
	for (int i = 0; i < HEALTH_PACK_STASH_AMOUNT; i++)
	{
		int r = rand() % rooms_for_summoning.size();
		Room* room = rooms_for_summoning.at(r);
		rooms_for_summoning.erase(rooms_for_summoning.begin() + r);
		int row = room->getCenterY() - room->getHeight() / 2 + (rand() % room->getHeight());
		int col = room->getCenterX() - room->getWidth() / 2 + (rand() % room->getWidth());
		maze[row][col] = HP_STASH;
		HP_Stashes.push_back(Position{ row, col });
	}
}

void initSoldierTeams()
{
	vector<array<double, 3>> colors1 = { team_colors.at("FPurple"), team_colors.at("SPurple") };
    vector<array<double, 3>> colors2 = { team_colors.at("FOrange"), team_colors.at("SOrange") };
    vector<vector<array<double, 3>>> colors = { colors1, colors2 };

	for (int i = 0; i < TEAM_NUM; i++)
	{
		Team* t = new Team(colors.at(i));
		int r = rand() % rooms_for_summoning.size();
		Room* room = rooms_for_summoning.at(r);
		rooms_for_summoning.erase(rooms_for_summoning.begin() + r);
		for (int j = 0; j < TEAM_SIZE; j++) {
			int row = room->getCenterY() - room->getHeight() / 2 + (rand() % room->getHeight());
			int col = room->getCenterX() - room->getWidth() / 2 + (rand() % room->getWidth());
			Position p = { row , col };
			t->addSoldier(p, true);
		}
		t->addSoldier(Position{
			room->getCenterY() - room->getHeight() / 2 + (rand() % room->getHeight()),
			room->getCenterX() - room->getWidth() / 2 + (rand() % room->getWidth()) 
			}, 
			false);
		Team::Teams.push_back(t);
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
		rooms_for_summoning.push_back(rooms[i]);
	}

	for (i = 0; i < NUM_OBSTACLES; i++)
	{
		maze[rand() % MSZ][rand() % MSZ] = WALL;
	}

	BuildPathBetweenRooms();

	initSoldierTeams();
	for (Team* t : Team::Teams) 
	{
		for (Soldier* s : t->getSoldiers())
		{
			Position p = s->getPos();
			maze[p.row][p.col] = SOLDIER;
		}
	}
	initAmmoAndHealthPackStashes();
}

void renderBitmapString(float x, float y, void* font, const string& str) {
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
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
			case AMMO_STASH:
				glColor3d(8 / (double)255, 255 / (double)255, 8/(double)255); // dark green
				break;
			case HP_STASH:
				glColor3d(214 / (double)255, 0, 28 / (double)255); // christmas red
				break;
			case SOLDIER:
				//glColor3d(1, 0, 0); // red
				for (Team* t : Team::Teams) {
					for (Soldier* s : t->getSoldiers()) {
						if (s->getPos().row == i && s->getPos().col == j) {
							if(!strcmp(s->getType(), "Fighter"))
								glColor3d(t->getFighterColor()[0], t->getFighterColor()[1], t->getFighterColor()[2]);
							else
								glColor3d(t->getSquireColor()[0], t->getSquireColor()[1], t->getSquireColor()[2]);
						}

					}
				}
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
			delete g;
		}
	}
}

void GenerateSecurityMapForSpsificTeam(int TeamNum)
{
	for (Team* t : Team::Teams) 
	{
		if (t->getTeamID().team == TeamNum)
		{
			continue;
		}
		//double* clonedMap = new double[MSZ * MSZ]();
		//double clonedMap[MSZ][MSZ] = { 0 };
		//cloneSecurityMap(security_map, clonedMap);
		cloneSecurityMapToPtr(security_map, security_maps.at(TeamNum));
        double (*clonedMap)[MSZ] = reinterpret_cast<double (*)[MSZ]>(security_maps.at(TeamNum));
		for (Soldier* s : t->getSoldiers())
		{
			Grenade* g = new Grenade(s->getPos().row, s->getPos().col);
			
			g->simulateExplosion(maze, clonedMap);

			delete g;
			g = nullptr;
		}
		cloneSecurityMapToPtr(clonedMap, security_maps.at(TeamNum));
	}
}

void drawHUD()
{
	float y_offset = MSZ - 2;
    float x_offset = 1; 

    glColor3d(0, 0, 0); 
    int soldierIndex = 1;

    for (Team* t : Team::Teams) {
        for (Soldier* s : t->getSoldiers()) {
            stringstream ss;
            ss << " [" << (dynamic_cast<Fighter*>(s) ? "Fighter" : "Squire") << "] "
               << "HP: " << s->getHP() << " Ammo: " << s->getAmmo() << " State: " << s->getState()->toString()
				<< " Team: " << ((t->getTeamID().team == 0) ? "Purple":"Orange");
            renderBitmapString(x_offset, y_offset,GLUT_BITMAP_HELVETICA_10, ss.str());

            y_offset -= 2; // Move down for next soldier
            if (y_offset < MSZ - 10) break; // Prevent overlapping
        }
		x_offset += MSZ / 2;
		y_offset = MSZ - 2;
		soldierIndex = 1;
    }
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	ShowDungeon();
	drawHUD();

	if (pb != nullptr)
	{
		pb->show();
	}
	if (pg != nullptr)
	{
		pg->show();
	}
	for (Bullet* b : Bullet::bullets)
		b->show();

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

	//srand(time(0));

	SetupDungeon();

	GenerateSecurityMap();
	initSecurityMaps();
}

void cloneAllSecMaps() {
	for (size_t i = 0; i < security_maps.size(); i++) {
		for (size_t j = 0; j < MSZ; j++) {
			for (size_t k = 0; k < MSZ; k++) {
				security_maps.at(i)[MSZ * j + k] = security_map[j][k];
			}
		}
	}
}

void idle() 
{
	Sleep(75);
	if (bulletFired)
	{
		pb->move(maze);
	}
	if (grenadeThrown)
	{
		pg->expand(maze);
	}

	if (!paused)
	{
		cloneAllSecMaps();
		for (size_t i = 0; i < security_maps.size(); i++)
			GenerateSecurityMapForSpsificTeam(i);

		for (Team* t : Team::Teams)
		{
			vector<Soldier*>& mey = t->getSoldiers();
			for (Soldier* s : mey)
			{
				s->getState()->OnEnter(s);
			}
		}
		for (Bullet* b : Bullet::bullets)
		{
			b->move(maze);
		}
		for (Grenade* g : Grenade::grenades)
		{
			g->expand(maze);
		}

		Bullet::bullets.erase(
			remove_if(Bullet::bullets.begin(), Bullet::bullets.end(), [](Bullet* b) {
				if (!b->getIsMoving()) {
					delete b;
					b = nullptr;
					return true;
				}
				return false;
			}),
			Bullet::bullets.end()
		);
		Grenade::grenades.erase(
			remove_if(Grenade::grenades.begin(), Grenade::grenades.end(), [](Grenade* g) {
				if (!g->getIsExpending()) {
					delete g;
					g = nullptr;
					return true;
				}
				return false;
				}),
			Grenade::grenades.end()
		);
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
		init();
	}
	if (key == 'g')
		setGridlines = !setGridlines;
	if (key == 's')
		showSecurityMap = !showSecurityMap;
	if (key == ' ')
		paused = !paused;
	if (key == 'q')
		exit(0);
}

LONG MyFilter(LONG excode) {
	return (excode == EXCEPTION_ACCESS_VIOLATION) ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
}

int main(int argc, char* argv[]) 
{
	__try
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
	__except(MyFilter(GetExceptionCode())) {
		std::cerr << "Access violation caught!" << std::endl;
		// Handle the exception (e.g., clean up resources)
	}

	return 0;
}