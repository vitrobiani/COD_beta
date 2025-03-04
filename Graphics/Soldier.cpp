#include "Soldier.h"

vector<vector<Soldier*>> Soldier::Teams(TEAM_NUM);
Soldier::Soldier(Position start_pos, TeamID tid)
{
	pos = start_pos;
	hp = START_HP;
	id = tid;
	isMoving = false;
	state = nullptr;
}

Position Soldier::findNearestEnemy()
{
	double distance = INFINITY;
	Position nearest = {};
	for (int i = 0; i < Teams.size(); i++) {
		if (i == id.team)
			continue;
		for (Soldier* s : Teams.at(i)) {
			double check = calculateDistance(s->getPos(), pos);
			if (check < distance) {
				nearest = s->getPos();
				distance = check;
			}
		}
	}
	return nearest;
}

double Soldier::calculateDistance(Position p1, Position p2)
{
	return sqrt(pow(p1.row - p2.row, 2) + pow(p1.col - p2.col, 2));
}

Cell* Soldier::runAS(int maze[MSZ][MSZ], double* security_map, Position target) {
    CompareCells comparator;
    priority_queue<Cell*, vector<Cell*>, CompareCells> grays(comparator);
    grays.push(new Cell(getPos().row, getPos().col, target.row, target.col, 0, nullptr, security_map));

    Cell* to_go_to = nullptr;
	while (!to_go_to) {
		to_go_to = ASIteration(grays, maze, target);
	}
    return to_go_to;
}

Cell* Soldier::ASIteration(priority_queue<Cell*, vector<Cell*>, CompareCells>& grays, int maze[MSZ][MSZ], Position target) {
    if (grays.empty()) {
        return nullptr;
    }

    Cell* pCurrent = grays.top();
    grays.pop();

    int row = pCurrent->getRow();
    int col = pCurrent->getCol();
    int target_row = target.row;
    int target_col = target.col;

    maze[row][col] = BLACK;

    Cell* go_on = nullptr;
    // Check neighbors
    if (maze[row + 1][col] == SPACE || maze[row + 1][col] == SOLDIER)
        go_on = CheckNeighbor(row + 1, col, pCurrent, target, maze, grays);
    if (!go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == SOLDIER))
        go_on = CheckNeighbor(row - 1, col, pCurrent, target, maze, grays);                                
    if (!go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == SOLDIER))
        go_on = CheckNeighbor(row, col - 1, pCurrent, target, maze, grays);                               
    if (!go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == SOLDIER))
        go_on = CheckNeighbor(row, col + 1, pCurrent, target, maze, grays);

    return go_on;
}

Cell* Soldier::CheckNeighbor(int row, int col, Cell* pCurrent, Position target, int maze[MSZ][MSZ], priority_queue<Cell*, vector<Cell*>, CompareCells>& grays) {
    if (col == target.col && row == target.row) {
        return RestorePath(pCurrent);
    }

    if (maze[row][col] == SPACE || maze[row][col] == SOLDIER) {
        Cell* pc = new Cell(row, col, target.row, target.col, 0, pCurrent, pCurrent->getSecurityMap());
        maze[row][col] = GRAY;
        grays.push(pc);
    }
    return nullptr;
}

Cell* Soldier::RestorePath(Cell* pc) {
    while (pc->getParent() && pc->getParent()->getParent() != nullptr) {
        pc = pc->getParent();
    }
    return pc; // Return the next step
}

bool Soldier::isEnemyInSight(Position enemy_pos) 
{
	for (int i = 0; i < IS_ENEMY_IN_SIGHT; i++)
	{
		Grenade* g = new Grenade(getPos().row, getPos().col);

		if(g->findEnemyByExplosion(maze, enemy_pos))
			return true;
        else
			continue;
	}
	return false;
}

void Soldier::move()
{
	if (isMoving)
	{
		Position enemy_pos = findNearestEnemy();
		int clonedMaze[MSZ][MSZ] = { 0 };
		cloneMaze(maze, clonedMaze);
		maze[getPos().row][getPos().col] = SPACE;
		Cell* c = runAS(clonedMaze, security_maps.at(getID().team), enemy_pos);
		maze[c->getRow()][c->getCol()] = SOLDIER;
		setPos(Position{ c->getRow(), c->getCol() });
		if (isEnemyInSight(enemy_pos))
			state->Transition(this); // Transition to attack state
	}
}


Cell* Soldier::runBFS(int maze[MSZ][MSZ], int curRow, int curCol) {
    int dupMaze[MSZ][MSZ] = {0};
    int dupMaze2[MSZ][MSZ] = {0};
    cloneMaze(maze, dupMaze);
    cloneMaze(maze, dupMaze2);
    queue<Cell*> grays;
    grays.push(new Cell(, nullptr));
    vector<Cell*> oppCells;

    Cell* to_go_to = nullptr;

    int i = 0;
    while (i < BFS_DEPTH || !oppCells.empty()) {
        to_go_to = BFSIteration(grays, maze, false);
        if (to_go_to) 
            oppCells.push_back(to_go_to);
        i++;
    }

	queue<Cell*> grays2;
	grays2.push(new Cell(, , nullptr));
	vector<double> heuristics;
	vector<Cell*> cells;
	int j = 0;
	while (j < BFS_DEPTH) {
		to_go_to = BFSIteration(grays2, dupMaze, walkable, true);
		if (to_go_to) {
			double h = 0;
			for (int i = 0; i < ghostCells.size(); i++) {
				h += Distance(ghostCells.at(i)->getRow(), ghostCells.at(i)->getCol(), to_go_to->getRow(), to_go_to->getCol());
			}
			heuristics.push_back(h);
			cells.push_back(to_go_to);
		}
		j++;
	}
	double max = heuristics.at(heuristics.size()-1);
	to_go_to = cells.at(heuristics.size()-1);
	for (int i = heuristics.size()-1; i > 0; i--) {
		if (heuristics.at(i) > max) {
			max = heuristics.at(i);
			to_go_to = cells.at(i);
		}
	}
	return PacmanRestorePath(to_go_to);
    return to_go_to;
}

Cell* Soldier::BFSIteration(queue<Cell*>& grays, int maze[MSZ][MSZ], vector<int> targets, bool forH) {
    if (grays.empty()) {
        return nullptr;
    }

    Cell* pCurrent = grays.front();
    grays.pop();

    int row = pCurrent->getRow();
    int col = pCurrent->getCol();

    if (maze[row][col] != START) {
        maze[row][col] = BLACK;
    }

    Cell* go_on = nullptr;
    // Check neighbors
    if (maze[row + 1][col] == SPACE || maze[row + 1][col] == COIN || isTarget(targets, maze[row + 1][col]) ) 
        go_on = PacmanCheckNeighbor(row + 1, col, pCurrent, targets, maze, grays, forH);
    if ((!go_on || forH) && (maze[row - 1][col] == SPACE || maze[row - 1][col] == COIN || isTarget(targets, maze[row - 1][col])))
        go_on = PacmanCheckNeighbor(row - 1, col, pCurrent, targets, maze, grays, forH);
    if ((!go_on || forH) && (maze[row][col - 1] == SPACE || maze[row][col - 1] == COIN || isTarget(targets, maze[row][col - 1])))
		go_on = PacmanCheckNeighbor(row, col - 1, pCurrent, targets, maze, grays, forH);
    if ((!go_on || forH) && (maze[row][col + 1] == SPACE || maze[row][col + 1] == COIN || isTarget(targets, maze[row][col + 1])))
		go_on = PacmanCheckNeighbor(row, col + 1, pCurrent, targets, maze, grays, forH);


    return go_on;
}

Cell* Pacman::PacmanCheckNeighbor(int row, int col, Cell* pCurrent, vector<int> targets, int maze[HEIGHT][WIDTH], queue<Cell*>& grays, bool forH) {
    if (isTarget(targets, maze[row][col])) {
        return pCurrent;
    }

    if (maze[row][col] == SPACE || maze[row][col] == COIN) {
        Cell* pc = new Cell(row, col, pCurrent);
        maze[row][col] = GRAY;
        grays.push(pc);
        if (forH) return pCurrent;
    }
    return nullptr;
}

Cell* Pacman::PacmanRestorePath(Cell* pc) {
    while (pc->getParent() && pc->getParent()->getParent() != nullptr) {
        pc = pc->getParent();
    }
    return pc;
}


