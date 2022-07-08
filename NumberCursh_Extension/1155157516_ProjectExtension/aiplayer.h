#ifndef _aiplayer_H_
#define _aiplayer_H_

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct swapList{
	Coord pos[100];
	int mean[100];
	int size;
}SwapList;

// main programmmmmmmmmm
int ai_player( int k, const char * board);

// board related
char boardElement(int, int, const char *);

// build array
int getEffSwapList(char *, int[][10]);
int boardScore(int, int, const char *);
int simWithRan(char *, int, int, int);

// real dumb AI
int dumbAI(int[][10], int);

#endif
