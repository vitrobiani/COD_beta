#include "Team.h"
#include "Fighter.h"

vector<Team*> Team::Teams;
//queue<Soldier*> Team::callingSquires;
Team::Team(vector<array<double, 3>> tc)
{
	teamColor = tc;
	teamID = generateTeamID();
	teamSize = 0;
}

TeamID Team::generateTeamID()
{
	static int team = 0;
	TeamID tid = { team++, 0 };
	return tid;
}

TeamID Team::generateTeamIDForSoldier()
{
	TeamID tid = { getTeamID().team , teamSize++};
	return tid;
}

void Team::addSoldier(Position start_pos, bool isFighter)
{
	if (isFighter) 
		soldiers.push_back(new Fighter(start_pos, generateTeamIDForSoldier()));
	else 
		soldiers.push_back(new Squire(start_pos, generateTeamIDForSoldier()));
}

Position Team::findNearestEnemy(Soldier* s)
{
	double distance = INFINITY;
	Position nearest = {};
	for (Team* t : Teams)
	{
		if (t->getTeamID().team == s->getID().team)
			continue;
		for (Soldier* sol : t->getSoldiers()) {
			double check = calculateDistance(sol->getPos(), s->getPos());
			if (check < distance) {
				nearest = sol->getPos();
				distance = check;
			}
		}
	}
	return nearest;
}

Position Team::findNearestTeammate(Soldier* s)
{
	double distance = INFINITY;
	Position nearest = {};
	for (Soldier* sol : Teams.at(s->getID().team)->getSoldiers()) {
		if (sol->getID().place == s->getID().place)
			continue;
		double check = calculateDistance(sol->getPos(), s->getPos());
		if (check < distance) {
			nearest = sol->getPos();
			distance = check;
		}
	}
	return nearest;
}

Soldier* Team::findNearestTeammatePtr(Soldier* s)
{
	double distance = INFINITY;
	int index = 0;
	int nearest = 0;
	for (Soldier* sol : Teams.at(s->getID().team)->getSoldiers()) {
		if (sol->getID().place == s->getID().place)
			continue;
		double check = calculateDistance(sol->getPos(), s->getPos());
		if (check < distance) {
			nearest = index;
			distance = check;
		}
		index++;
	}
	return Teams.at(s->getID().team)->getSoldiers().at(nearest);
}

double Team::calculateDistance(Position p1, Position p2)
{
	return sqrt(pow(p1.row - p2.row, 2) + pow(p1.col - p2.col, 2));
}

vector<Position> Team::getSoldiersPositions() {
	vector<Position> positions;
	for (Soldier* s : soldiers) {
		positions.push_back(s->getPos());
	}
	return positions;
}

void Team::getEnemiesPositions(vector<Position>& positions) {
	for (Team* t : Teams) {
		if (t->getTeamID().team == teamID.team)
			continue;
		vector<Soldier*>& mey = t->getSoldiers();
		for (Soldier* s : mey) {
			positions.push_back(s->getPos());
		}
	}
}

Team* Team::getTeamByID(TeamID id) {
	for (Team* t : Teams) {
		if (t->getTeamID().team == id.team)
			return t;
	}
}

Position Team::findSafestPosition(int maze[MSZ][MSZ], Soldier* s, double* sec_map)
{
	Cell* c = findBestHeuristicCell(maze, s, sec_map);
	c = RestorePath(c);
	return Position{ c->getRow(), c->getCol() };
}

Cell* Team::findBestHeuristicCell(int maze[MSZ][MSZ], Soldier* s, double* sec_map) {
    cloneMaze(maze, dupMaze);
	vector<Position> targets;
	targets.reserve(4);
	Team::getTeamByID(s->getID())->getEnemiesPositions(targets);

    queue<Cell*> grays;
    grays.push(new Cell(s->getPos().row, s->getPos().col, nullptr));
    Cell* to_go_to = nullptr;
    vector<double> heuristics;
    vector<Cell*> cells;
    int j = 0;
    while (j < BFS_DEPTH) {
        to_go_to = BFSIteration(grays, dupMaze);
        double h = 0;
        // heuristic calculation takes into account the distance from the soldier to the enemies and the security map
        for (Position p : targets) { 
            h += Team::calculateDistance(Position{to_go_to->getRow(), to_go_to->getCol()}, p); // higher = more distant = better
            double security = sec_map[MSZ * p.row + p.col]; // higher = less secure = worse
            h -= security * SECURITY_COEFFICIENT*100;
        }
        heuristics.push_back(h);
        cells.push_back(to_go_to);
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
    //delete[] dupMaze;
    //delete[] dupMaze2;
    return to_go_to;
}

Cell* Team::BFSIteration(queue<Cell*>& grays, int maze[MSZ][MSZ]) {
    if (grays.empty()) {
        return nullptr;
    }

    Cell* pCurrent = grays.front();
    grays.pop();

    int row = pCurrent->getRow();
    int col = pCurrent->getCol();

	maze[row][col] = BLACK;

    // Check neighbors
    if (maze[row + 1][col] == SPACE || maze[row + 1][col] == SOLDIER)
		CheckNeighbor(Position{ row + 1, col }, pCurrent, maze, grays);
    if (maze[row - 1][col] == SPACE || maze[row - 1][col] == SOLDIER)
        CheckNeighbor(Position {row - 1, col}, pCurrent, maze, grays);
    if (maze[row][col - 1] == SPACE || maze[row][col - 1] == SOLDIER)
		CheckNeighbor(Position {row, col - 1}, pCurrent, maze, grays);
    if (maze[row][col + 1] == SPACE || maze[row][col + 1] == SOLDIER)
		CheckNeighbor(Position {row, col + 1}, pCurrent, maze, grays);


    return pCurrent;
}

void Team::CheckNeighbor(Position p, Cell* pCurrent, int maze[MSZ][MSZ], queue<Cell*>& grays) {
	Cell* pc = new Cell(p.row, p.col, pCurrent);
	maze[p.row][p.col] = GRAY;
	grays.push(pc);
}

Cell* Team::RestorePath(Cell* pc) {
    while (pc->getParent() && pc->getParent()->getParent() != nullptr) {
        pc = pc->getParent();
    }
    return pc;
}


