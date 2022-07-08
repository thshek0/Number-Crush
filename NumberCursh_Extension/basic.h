#ifndef _basic_H_
#define _basic_H_

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct coord{
	int swapPos1, swapPos2;
}Coord;

typedef struct preference{
	int k;
	int isShowStep;
	int player;
	int isStone;
}Preference;

// 	initialize
void initBoard(char[], int);
// 	input
int getInput(const char *);
int isValidInput(int, const char *);
// 	process
void swap(int, int , char *);
int isCombo(int, int, int, const char *);
int elimination(char *, int);
void gravity(char *);
void placeRandom(char *, int);
// 	output
void printInfo(int, int, const int *, const char *, int);
void printBoard(const char *);

#endif
