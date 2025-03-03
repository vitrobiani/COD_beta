#pragma once
#include <math.h>
#include "definitions.h"

class Cell
{
private:
	int row, col;
	int target_row, target_col;
	Cell* parent;
	double f, g, h;
	double* security_map;
public:
	Cell();
	Cell(int r, int c, int tr, int tc, double newg, Cell* p, double* sec_map);
	int getRow() { return row; }
	int getCol() { return col; }
	Cell* getParent() { return parent; }
	void ComputeH();
	double getF() { return f; }
	double getH() { return h; }
	double getG() { return g; }
	int getTargetRow() { return target_row; }
	int getTargetCol() { return target_col; }
	double* getSecurityMap() { return security_map; }

	bool operator==(const Cell& other) { return row == other.row && col == other.col; }
};

