#include "Soldier.h"

Soldier::Soldier(Position start_pos, TeamID tid)
{
	pos = start_pos;
	hp = START_HP;
	id = tid;
	isMoving = false;
	state = nullptr;
}

void Soldier::move(Position p)
{
    if (isMoving)
    {
        maze[getPos().row][getPos().col] = SPACE;
        maze[p.row][p.col] = SOLDIER;
        setPos(p);
    }
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

void Soldier::moveToEnemy(Position enemy_pos)
{
	if (isMoving)
	{
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

bool isBulletCloseEnough(Position p1, Position p2)
{
	return sqrt(pow(p1.row - p2.row, 2)+pow(p1.col - p2.col, 2))<= 5;
}

bool Soldier::checkHitByBullet()
{
    for (Bullet* b : Bullet::bullets) {
        if (b->getIsMoving() && isBulletCloseEnough(b->getPos(), getPos()))
        {
            hp -= 10;
            //b->setIsMoving(false);
            return true;
        }
    }
    return false;
}
