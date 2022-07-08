#ifndef _basic_1155157516_H_
#define _basic_1155157516_H_

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

// 	initialize
void initBoard();
// 	input
int getInput();
int isValidInput(int);
// 	process
void swap(int);
int eliminate();
int isHorCombo(int, int, int[]);
int isVerCombo(int, int, int[]);
void gravity();
void placeRandom();
// 	output
void printInfo(int);

#endif

