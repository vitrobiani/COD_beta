#include "Cell.h"

Cell::Cell()
{
	parent = nullptr;
	col = 0;
	row = 0;
	target_col = 0;
	target_row = 0;
	h = 0;
	g = 0;
	f = 0;
}

// gets row, col, target row, target col, parent g and parent itself
Cell::Cell(int r, int c, int tr, int tc, double newg, Cell* p, double* sec_map)
{
	row = r;
	col = c;
	parent = p;
	target_row = tr;
	target_col = tc;
	g = newg;
	ComputeH();
	f = h + g;
	security_map = sec_map;
}

void Cell::ComputeH()
{
	if (!security_map) {
		h = abs(row - target_row) + abs(col - target_col);
		return;
	}
	double dis = sqrt(pow(row - target_row, 2) + pow(col - target_col, 2));
	double security = security_map[MSZ * row + col];
	h = dis + security * SECURITY_COEFFICIENT;
}
