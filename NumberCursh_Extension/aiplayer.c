#include "basic.h"
#include "aiplayer.h"

int ai_player( int k, const char * board ){
	// copy gameboard
	char testBoard[105] = "";
	strcpy(testBoard, board);
	//	posEffSwap[][index] meaning:
	//	0-1: 	2 numbers to be swapped
	//	2: 		score of first batch
	//	3:		total mean score of simulation with random
	int posEffSwap[100][10] = {0};
	int size = 0;
	int i = 0, j = 0;
	// 	get list of effective swap with score
	size = getEffSwapList(testBoard, posEffSwap);
	// 	simulate each possible scorable swap
	// 	simulation result passes to array
	for(i = 0; i < size; i++){
		for(j = 0; j < 500; j++){
			strcpy(testBoard, board);
			posEffSwap[i][3] += simWithRan(testBoard, posEffSwap[i][0], posEffSwap[i][1], k);
		}
	}
	return dumbAI(posEffSwap, size);
}

//	parameter: x,y_coordinate, gameboard
//	access array with check (prevent array out of bound error)
//	return value: if valid pos -> return element of array
//				  if invalid pos -> return -1
char boardElement(int x, int y, const char * gameBoard){
	if( x < 0 || x > 9 || y < 0 || y > 9 ){
		return -1;
	}else{
		return gameBoard[10 * x + y];
	}
}

//	parameter: 2 coordinate, gameboard
//	obtain score from isCombo
//	return total score
int boardScore(int coord1, int coord2, const char * gameBoard){
	int x1 = coord1 / 10;
	int y1 = coord1 % 10;
	int x2 = coord2 / 10;
	int y2 = coord2 % 10;
	int combo[4] = {0};
	int i = 0, score = 0;
	
	for(i = 0; i < 4; i++){
		if(i / 2 == 0){
			combo[i] = isCombo(x1, y1, i % 2 * 2, gameBoard) + isCombo(x1, y1, i % 2 * 2 + 1, gameBoard) - 1;
		}else{
			combo[i] = isCombo(x2, y2, i % 2 * 2, gameBoard) + isCombo(x2, y2, i % 2 * 2 + 1, gameBoard) - 1;
		}
	}
	
	if( x1 == x2 && gameBoard[coord1] == gameBoard[coord2 + 1] ){
		combo[2] = 0;
	}else if( y1 == y2 && gameBoard[coord1] == gameBoard[coord2 + 10] ){
		combo[3] = 0;
	}
	for(i = 0; i < 4; i++){
		if(combo[i] >= 3){
			score += combo[i] * combo[i];
		}
	}
	return score;
}

//	THE REAL AI PART!!!
//	parameter: array with all score recorded, size of that array, index to be accessed
//	find possible max score
//	return move that makes max avg score
int dumbAI(int pos[][10], int size){
	int i = 0;
	int max = 0, maxidx = 0;
	for(i = 0; i < size; i++){
		if(pos[i][3] >= max){
			max = pos[i][3];
			maxidx = i;
		}
	}
	//	if no possible scorable move
	int temp1 = 0, temp2 = 0;
	if(size == 0){
		temp1 = rand() % 10;
		temp2 = rand() % 9;
		return 1010 * temp1 + 101 * temp2 + 1;
	}
	
	return 100 * pos[maxidx][0] + pos[maxidx][1];
}

//	get effective swap list by swapping each element
//	parameter: gameBoard, array to store the result
//	return value: size of array
int getEffSwapList(char * gameBoard, int posEffSwap[][10]){
	int i = 0, cnt = 0, score = 0;
	for(i = 0; i < 100; i++){
		// horizontal swap
		if(i % 10 != 9){
			if(gameBoard[i] != '@' && gameBoard[i+1] != '@'){
				swap(i, i + 1, gameBoard);
				score = boardScore(i, i + 1, gameBoard);
				if(score != 0){
					posEffSwap[cnt][0] = i;
					posEffSwap[cnt][1] = i + 1;
					cnt++;
				}
				swap(i, i + 1, gameBoard);
			}
			
		}
		// vertical swap
		if(i < 90){
			if(gameBoard[i] != '@' && gameBoard[i+10] != '@'){
				swap(i, i + 10, gameBoard);
				score = boardScore(i, i + 10, gameBoard);
				if(score != 0){
					posEffSwap[cnt][0] = i;
					posEffSwap[cnt][1] = i + 10;
					cnt++;
				}
				swap(i, i + 10, gameBoard);
			}
		}
	}
	return cnt;
}

// 	simulate just like the basic part
//	parameter: gameboard, swapping array, index, k
//	store newly simulated score into posEffSwap
//	no return value
int simWithRan(char * gameBoard, int x, int y, int k){
	int score = 0, totalScore = 0;
	swap(x, y, gameBoard);
	do{
		score = elimination(gameBoard, 0);
		gravity(gameBoard);
		placeRandom(gameBoard, k);
		totalScore += score;
	}while(score != 0);
	
	return totalScore;	
}
