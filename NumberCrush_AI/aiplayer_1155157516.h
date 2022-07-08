#ifndef _aiplayer_1155157516_H_
#define _aiplayer_1155157516_H_

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

// main programmmmmmmmmm
int ai_player_1155157516( int k, const int * board);

// board related
int boardElement_1155157516(int, int, const int *);
int * duplicateBoard_1155157516(const int *);
void printBoard_1155157516(int *);
void swapPos_1155157516 (int, int, int *);
int isCombo_1155157516(int, int, int, const int *);
int boardScore_1155157516(int, int, const int *);

// simulation
int simElimination_1155157516(int *);
void simGravity_1155157516(int *);
void simRandom_1155157516(int *, const int);
// build array
int getEffSwapList_1155157516(int *, int[][10]);
//void simWithoutRan_1155157516(int *, int[][10], int);
void simWithRan_1155157516(int *, int[][10], int, int);

// real dumb AI
int dumbAI1_1155157516(int[][10], int, int);

#endif
