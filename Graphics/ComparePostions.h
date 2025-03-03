#pragma once
#include "definitions.h"
#include "Cell.h"

class ComparePositions
{
private:
	const Cell* target;
	const double* security_map;
public:
	ComparePositions(const double* sec_map, const Position* target_pos);
	//double calculateHeuristic(const Cell* cur, const Cell* target);

    //bool operator()(const Cell* pn1, const Cell* pn2) {
		//double h1 = calculateHeuristic(pn1, target);
		//double h2 = calculateHeuristic(pn2, target);
  //      return h1 > h2;
    //}
};


