#include "definitions.h"

void cloneMaze(int source[MSZ][MSZ], int target[MSZ][MSZ]) {
	for (int i = 0; i < MSZ; i++) {
		for (int j = 0; j < MSZ; j++) {
			target[i][j] = source[i][j];
		}
	}
}

void cloneSecurityMap(double source[MSZ][MSZ], double target[MSZ][MSZ]) {
	for (int i = 0; i < MSZ; i++) {
		for (int j = 0; j < MSZ; j++) {
			target[i][j] = source[i][j];
		}
	}
}

void cloneSecurityMapToPtr(double source[MSZ][MSZ], double* target) {
	for (int i = 0; i < MSZ; i++)
		for (int j = 0; j < MSZ; j++)
			target[MSZ * i + j] = source[i][j];
}
