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
