#include "aiplayer_1155157516.h"

int ai_player_1155157516( int k, const int * board ){
	
	srand(time(0));
	// copy gameboard
	int * testBoard = duplicateBoard_1155157516(board);
	//	posEffSwap[][index] meaning:
	//	0-1: 	2 numbers to be swapped
	//	2: 		score of first batch
	//	3:		score of simulation without random
	//	4-7:	score of simulation with random
	//			4 current, 5 worst, 6 total (mean), 7 best, 8 index
	int posEffSwap[100][10] = {0};
	int size = 0;
	int i = 0, j = 0;
	
	size = getEffSwapList_1155157516(testBoard, posEffSwap);
	
	// 	get list of effective swap with score
	for(i = 0; i < size; i++){
		testBoard = duplicateBoard_1155157516(board);
		//simWithoutRan_1155157516(testBoard, posEffSwap, i);
		/*for(j = 0; j <= 3; j++){
			printf("%d ", posEffSwap[i][j]);
		}
		printf("\n");*/
		//printBoard_1155157516(testBoard);
	}
	
	// 	simulate each possible scorable swap
	// 	simulation result passes to array
	for(i = 0; i < size; i++){
		for(j = 0; j < 1000; j++){
			testBoard = duplicateBoard_1155157516(board);
			simWithRan_1155157516(testBoard, posEffSwap, i, k);
		}
		/*for(j = 0; j <= 8; j++){
			printf("%d ", posEffSwap[i][j]);
		}
		printf("\n");*/
		//printBoard_1155157516(testBoard);
		//tempArr[i] = testBoard[i][6];
	}
	
	return dumbAI1_1155157516(posEffSwap, size, 6);
}

//	parameter: 2 coordinates, gameboard
//	swap the 2 elements in the gameboard
//	no return value
void swapPos_1155157516( int coord1, int coord2, int * gameBoard ){
	int temp = gameBoard[coord1];
	gameBoard[coord1] = gameBoard[coord2];
	gameBoard[coord2] = temp;
}

//	parameter: constant array
//	make a copy of an original array
//	return value: int pointer of copied array
int * duplicateBoard_1155157516( const int * gameBoard ){
	int i = 0;
	static int testBoard[105];
	for(i = 0; i < 100; i++){
		testBoard[i] = gameBoard[i];
	}
	return testBoard;
}

//	parameter: x,y_coordinate, gameboard
//	access array with check (prevent array out of bound error)
//	return value: if valid pos -> return element of array
//				  if invalid pos -> return -1
int boardElement_1155157516(int x, int y, const int * gameBoard){
	if( x < 0 || x > 9 || y < 0 || y > 9 ){
		return -1;
	}else{
		return gameBoard[10 * x + y];
	}
}

//	parameter: x,y_coordinate, direction of combo, gameboard
//	check can_be_eleiminated combo on board with chosen dir (hor or ver)
//	return combo (if >3, return combo, else, return 1)
int isCombo_1155157516(int x, int y, int dir, const int * gameBoard){
	int combo = 0;
	int i = 0, j = 0;
	if(dir == 0){
		j = -1;
	}else if(dir == 1){
		j = 1;
	}else if(dir == 2){
		i = -1;
	}else{
		i = 1;
	}
	//printf("%d %d\n", x+i, y+j);
	//printf("%d %d %d %d\n", x, y, x+i, y+j);
	if(boardElement_1155157516(x, y, gameBoard) == boardElement_1155157516(x+i, y+j, gameBoard)){
		//printf("Here %d %d\n", boardElement_1155157516(x, y, gameBoard), boardElement_1155157516(x+i, y+j, gameBoard));
		return isCombo_1155157516(x+i, y+j, dir, gameBoard) + 1;
	}else{
		return 1;
	}
}

//	parameter: 2 coordinate, gameboard
//	obtain score from isCombo
//	return total score
int boardScore_1155157516(int coord1, int coord2, const int * gameBoard){
	int x1 = coord1 / 10;
	int y1 = coord1 % 10;
	int x2 = coord2 / 10;
	int y2 = coord2 % 10;
	int combo[4] = {0};
	int i = 0, score = 0;
	
	for(i = 0; i < 4; i++){
		if(i / 2 == 0){
			combo[i] = isCombo_1155157516(x1, y1, i % 2 * 2, gameBoard) + isCombo_1155157516(x1, y1, i % 2 * 2 + 1, gameBoard) - 1;
		}else{
			combo[i] = isCombo_1155157516(x2, y2, i % 2 * 2, gameBoard) + isCombo_1155157516(x2, y2, i % 2 * 2 + 1, gameBoard) - 1;
		}
		//printf("Combo %d: %d\n", i, combo[i]);
	}
	if( x1 == x2 && gameBoard[coord1] == gameBoard[coord2 + 1] ){
		combo[2] = 0;
	}else if( y1 == y2 && gameBoard[coord1] == gameBoard[coord2 + 10] ){
		combo[3] = 0;
	}
	for(i = 0; i < 4; i++){
		//printf("%d ", combo[i]);
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
int dumbAI1_1155157516(int pos[][10], int size, int index){
	int i = 0;
	int max = 0, maxidx = 0;
	int temp1 = 0, temp2 = 0;
	/*for(i = 0; i < size; i++){
		printf("%d %d %d\n", pos[i][0], pos[i][1], pos[i][2]);
	}*/
	for(i = 0; i < size; i++){
		if(pos[i][index] >= max){
			max = pos[i][index];
			maxidx = i;
		}
	}
	//	if no possible scorable move
	if(size == 0){
		temp1 = rand() % 10;
		temp2 = rand() % 9;
		return 1010 * temp1 + 101 * temp2 + 1;
	}
	
	return 100 * pos[maxidx][0] + pos[maxidx][1];
}

//	simulate elimination just like the basic part
//	parameter: gameboard
//	return value: score of this elimination
int simElimination_1155157516(int * gameBoard){
	int i, j, k = 0;
	int temp = 0;
	int score = 0;
	// make a copy of gameBoard
	int copy1[100], copy2[100];
	for(i = 0; i < 100; i++){
		copy1[i] = gameBoard[i];
		copy2[i] = gameBoard[i];
	}
	// check horizontal and vertical combo
	for(i = 0; i < 100; i++){
		if(copy1[i] != 0 && i % 10 <= 7){
			temp = isCombo_1155157516(i / 10, i % 10, 1, copy1);
			if(temp >= 3){
				for(k = 0; k < temp; k++){
					copy1[i + k] = 0;
				}
				score += temp * temp;
			}
		}
		if(copy2[i] != 0 && i / 10 <= 7){
			temp = isCombo_1155157516(i / 10, i % 10, 3, copy2);
			if(temp >= 3){
				for(k = 0; k < temp; k++){
					copy2[i + 10 * k] = 0;
				}
				score += temp * temp;
			}
		}
	}
	// combine original and copied gameboard
	for(i = 0; i < 100; i++){
		if(copy1[i] == 0 || copy2[i] == 0){
			gameBoard[i] = 0;
		}
	}
	return score;
}

// 	simulate gravity just like the bsic part
//	parameter: gameboard
//	no return value
void simGravity_1155157516(int * gameBoard){
	// stack: move all 0 upward
	int index = 0;
	int i, j = 0;
	for(j = 0; j < 10; j++){
		index = 9;
		for(i = 9; i >= 0; i--){
			if(gameBoard[10 * i + j] != 0){
				gameBoard[10 * index + j] = gameBoard[10 * i + j];
				index--;
			}
		}
		while(index >= 0){
			gameBoard[10 * index + j] = 0;
			index--;
		}
	}
}

//	simulate random number just like the basic part
//	parameter: gameboard, k
//	no return value
void simRandom_1155157516(int * gameBoard, const int k){
	//srand(0);
	int i = 0;
	for(i = 0; i < 100; i++){
		if(gameBoard[i] == 0){
			gameBoard[i] = rand() % k + 1;
		}
	}
}

//	get effective swap list by swapping each element
//	parameter: gameBoard, array to store the result
//	return value: size of array
int getEffSwapList_1155157516(int * gameBoard, int posEffSwap[][10]){
	int i = 0, cnt = 0, temp = 0;
	for(i = 0; i < 100; i++){
		// horizontal swap
		if(i % 10 != 9){
			swapPos_1155157516(i, i + 1, gameBoard);
			temp = boardScore_1155157516(i, i + 1, gameBoard);
			if(temp != 0){
				posEffSwap[cnt][0] = i;
				posEffSwap[cnt][1] = i + 1;
				posEffSwap[cnt][2] = temp;
				cnt++;
			}
			swapPos_1155157516(i, i + 1, gameBoard);
		}
		
		// vertical swap
		if(i < 90){
			swapPos_1155157516(i, i + 10, gameBoard);
			temp = boardScore_1155157516(i, i + 10, gameBoard);
			if(temp != 0){
				posEffSwap[cnt][0] = i;
				posEffSwap[cnt][1] = i + 10;
				posEffSwap[cnt][2] = temp;
				cnt++;
			}
			swapPos_1155157516(i, i + 10, gameBoard);
		}
	}
	return cnt;
}

//	NOT TO BE USED ANYMORE
/*void simWithoutRan_1155157516(int * gameBoard, int posEffSwap[][10], int index){
	int score = 0;
	posEffSwap[index][3] = 0;
	swapPos_1155157516(posEffSwap[index][0], posEffSwap[index][1], gameBoard);
	do{
		score = simElimination_1155157516(gameBoard);
		simGravity_1155157516(gameBoard);
		posEffSwap[index][3] += score;
		//printBoard_1155157516(gameBoard);
	}while(score != 0);
	//swapPos_1155157516(posEffSwap[index][0], posEffSwap[index][1], gameBoard);
}*/

// 	simulate just like the basic part
//	parameter: gameboard, swapping array, index, k
//	store newly simulated score into posEffSwap
//	no return value
void simWithRan_1155157516(int * gameBoard, int posEffSwap[][10], int index, int k){
	int score = 0;	
	swapPos_1155157516(posEffSwap[index][0], posEffSwap[index][1], gameBoard);
	posEffSwap[index][4] = 0;
	do{
		score = simElimination_1155157516(gameBoard);
		simGravity_1155157516(gameBoard);
		simRandom_1155157516(gameBoard, k);
		posEffSwap[index][4] += score;
		//printBoard_1155157516(gameBoard);
	}while(score != 0);
	
	if( posEffSwap[index][8] == 1){
		posEffSwap[index][5] = posEffSwap[index][4];
		posEffSwap[index][7] = posEffSwap[index][4];
	}
	if( posEffSwap[index][4] > posEffSwap[index][7]){
		posEffSwap[index][7] = posEffSwap[index][4];
	}
	if( posEffSwap[index][4] < posEffSwap[index][5]){
		posEffSwap[index][5] = posEffSwap[index][4];
	}
	posEffSwap[index][6] += posEffSwap[index][4];	
	posEffSwap[index][8]++;
}

// just for testing
void printBoard_1155157516(int * gameBoard){
	printf("    0 1 2 3 4 5 6 7 8 9\n");
	printf("   --------------------\n");
	int i = 0, j = 0;
	for(i = 0; i < 10; i++){
		printf("%d | ", i);
		for(j = 0; j < 10; j++){
			printf("%d ", gameBoard[10 * i + j]);
		}
		printf("\n");
	}
}
