#include "Soldier.h"

Soldier::Soldier(Position start_pos, TeamID tid)
{
	pos = start_pos;
	hp = START_HP;
	id = tid;
	isMoving = false;
	state = nullptr;
	hp_th = (rand() % START_HP / 4) + BULLET_DAMAGE;
}

void Soldier::move(Position p)
{
    if (isMoving)
    {
        //if (maze[p.row][p.col] == SOLDIER)
        //    return;
        maze[getPos().row][getPos().col] = SPACE;
        maze[p.row][p.col] = SOLDIER;
        setPos(p);
    }
}

Position Soldier::runAS(int maze[MSZ][MSZ], double* security_map, Position target) {
    CompareCells comparator;
    priority_queue<Cell*, vector<Cell*>, CompareCells> grays(comparator);
    grays.push(new Cell(getPos().row, getPos().col, target.row, target.col, 0, nullptr, security_map));
	vector<Cell*> used;

    Cell* to_go_to = nullptr;
    int i = 0;
	while (!to_go_to) {
		to_go_to = ASIteration(grays, maze, target, used);
        i++;
	}

	Position p = Position { to_go_to->getRow(), to_go_to->getCol() };

    while (!grays.empty()) {
        Cell* cellToPop = grays.top();
        grays.pop();
        delete cellToPop;
		cellToPop = nullptr;
    }
    while (!used.empty()) {
		Cell* cellToPop = used.back();
        used.pop_back();
        delete cellToPop;
		cellToPop = nullptr;
    }

    return p;
}

Cell* Soldier::ASIteration(priority_queue<Cell*, vector<Cell*>, CompareCells>& grays, int maze[MSZ][MSZ], Position target, vector<Cell*>& used) {
    if (grays.empty()) {
        return nullptr;
    }

    Cell* pCurrent = grays.top();
    grays.pop();

    if (grays.size() == 0 && target.row == 44 && target.col == 17) {
        int x = 5;
    }

    int row = pCurrent->getRow();
    int col = pCurrent->getCol();
    int target_row = target.row;
    int target_col = target.col;
    if (row == target_row && col == target_col)
        return pCurrent;

    maze[row][col] = BLACK;

    Cell* go_on = nullptr;
    // Check neighbors
    if (maze[row + 1][col] == SPACE || maze[row + 1][col] == SOLDIER || maze[row + 1][col] == AMMO_STASH || maze[row + 1][col] == HP_STASH)
        go_on = CheckNeighbor(row + 1, col, pCurrent, target, maze, grays);
    if (!go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == SOLDIER || maze[row - 1][col] == AMMO_STASH || maze[row - 1][col] == HP_STASH))
        go_on = CheckNeighbor(row - 1, col, pCurrent, target, maze, grays);                                
    if (!go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == SOLDIER || maze[row][col - 1] == AMMO_STASH || maze[row][col - 1] == HP_STASH))
        go_on = CheckNeighbor(row, col - 1, pCurrent, target, maze, grays);                               
    if (!go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == SOLDIER || maze[row][col + 1] == AMMO_STASH || maze[row][col + 1] == HP_STASH))
        go_on = CheckNeighbor(row, col + 1, pCurrent, target, maze, grays);

	used.push_back(pCurrent);
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

void Soldier::hitByBullet()
{
	hp -= BULLET_DAMAGE;
}

