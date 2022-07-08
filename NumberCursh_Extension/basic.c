#include "basic.h"
#include "aiplayer.h"
#include "tinymt32.h"

tinymt32_t state;

int main(){
	uint32_t pesudo_seed = 0;
	tinymt32_init(&state, pesudo_seed);
	Coord pos;
	int input = 0;
	char gameBoard[105] = "";
	int iter = 0, cnt = 0;
	int player = 0;
	int stepScore = 0, batchScore = 0;
	int totalScore[3] = {0};
	const int K = 4;
	// 	init
	initBoard(gameBoard, K);
	// 	for every step:
	while( iter < 100 ){
		// 	print gameBoard status (step #, board element)
		printInfo(iter, 0, totalScore, gameBoard, 1);
		stepScore = 0;
		//	even iter -> human player turn, odd iter -> AI turn
		player = iter % 2;
		//player = 1;
		if(player){
			printf("AI Player's turn !!\n");
			input = ai_player(K, gameBoard);
		}else{
			printf("Human Player's turn !!\n");
			input = getInput(gameBoard);
		}
		pos.swapPos1 = input / 100;
		pos.swapPos2 = input % 100;
		//	swap 2 pos
		swap( pos.swapPos1, pos.swapPos2, gameBoard );
		printf("Numbers at %d and %d are swapped.\n", pos.swapPos1, pos.swapPos2);
		
		cnt = 1;
		do{
			//	eliminate + falling block + random generate + print board after each step
			batchScore = elimination(gameBoard, 1);
			if(batchScore != 0){
				stepScore += batchScore;
				printInfo(iter, cnt, totalScore, gameBoard, 2);
				gravity(gameBoard);
				printInfo(iter, cnt, totalScore, gameBoard, 3);
				placeRandom(gameBoard, K);
				printInfo(iter, cnt, totalScore, gameBoard, 4);
				cnt++;
			}
		}while(batchScore != 0);
		//	add each batch score to step score, then to total score of player
		printf("Score obtained in this step: %d\n", stepScore);
		totalScore[player] += stepScore;
		iter++;
	}
	//	terminate (output all scores)
	printf("Total score ( Human Player ): %d\n", totalScore[0]);
	printf("Total score ( AI Player )   : %d\n", totalScore[1]);
	printf("Mean score : %lf", ( (double) (totalScore[0] + totalScore[1]) / 2) );
	
	return 0;
}

//	parameter: gameBoard, K
//  1. generate gameBoard all by random
//	2. eliminate until no more combo appears in init. board
//	3. generate random number of stone block
//	4. copy temp gameboard to the REAL gameBoard
//	return value: none
void initBoard(char gameBoard[], int K){
	//char temp[105] = "1234323234234234141142321141412343341214433231232221434141242112142433424331211321144211411342124213";
	char temp[105] = "";
	int i = 0, tempScore = 0, numRan = 0;
	// generate random number
	for(i = 0; i < 100; i++){
		temp[i] = tinymt32_generate_uint32(&state) % K + '1';
	}
	temp[100] = '\0';
	// to ensure no combo created
	do{
		tempScore = elimination(temp, 0);
		if(tempScore != 0){
			gravity(temp);
			placeRandom(temp, K);
		}
	}while(tempScore != 0);
	// generate random number of stone block '@' at random location
	numRan = tinymt32_generate_uint32(&state) % 7 + 4;
	for(i = 0; i < numRan; i++){
		temp[ tinymt32_generate_uint32(&state) % 100 ] = '@';
	}
	strcpy(gameBoard, temp);
}	

// 	parameter: gameBoard
// 	get input from user
//	keep asking input intil it gets a valid input
//	check input using isValidInput(input);
//	return value: validated input
int getInput(const char * gameBoard){
	int input = 0;
	do{
		printf("Please enter swapping locations:\n");
		scanf("%d", &input);
	}while( !isValidInput(input, gameBoard) );
	return input;
}

//	parameter: input from getInput (not validate yet), gameBoard
// 	check whether the input is valid and print warning message for:
// 	1. input in range 0000 - 9999
//	2. 2 numbers is adjacent to each others
//	3. the swapping involves stone block
//	return value: 1(true) for valid, 0(false) for invalid
int isValidInput(int input, const char * gameBoard){
	if(input < 0 || input > 9999){
		printf("Invalid input. Try again. (Invalid input range)\n");
		return 0;
	}
	if( abs(input / 1000 - (input / 10) % 10) + abs((input / 100) % 10 - input % 10) != 1){
		printf("Invalid input. Try again. (Attempt to swap non-adjacent position)\n");
		return 0;
	}
	if(gameBoard[input / 100] == '@' || gameBoard[input % 100] == '@'){
		printf("Invalid input. Try again. (Attempt to move stone block)\n");
		return 0;
	}
	return 1;
}

// 	parameter: validated input number (e.g. 1112), gameBoard
// 	task: swap elements in gameBoard, e.g. 11 and 12
//	return value: none
void swap(int num1, int num2, char * gameBoard){
	char temp = gameBoard[num1];
	gameBoard[num1] = gameBoard[num2];
	gameBoard[num2] = temp;
}

//	parameter: gameboard, print
//	check if there are combo by isCombo (hor then ver)
//	if yes, then change combo elements to 0
//	if print = 1, print messages, if print = 0 otherwise
//	return value: score of this elimination
int elimination(char * gameBoard, int print){
	int i, j, k = 0;
	int score = 0;
	int temp = 0;
	// make a copy of gameBoard
	char copy1[100], copy2[100];
	strcpy(copy1, gameBoard);
	strcpy(copy2, gameBoard);
	
	// check horizontal and vertical combo
	for(i = 0; i < 100; i++){
		if(copy1[i] != '0' && i % 10 <= 7){
			temp = isCombo(i / 10, i % 10, 1, copy1);
			if(temp >= 3){
				for(k = 0; k < temp; k++){
					copy1[i + k] = '0';
				}
				if(print){
					printf("%d numbers are eliminated!\n", temp);
				}
				score += temp * temp;
			}
		}
		if(copy2[i] != '0' && i / 10 <= 7){
			temp = isCombo(i / 10, i % 10, 3, copy2);
			if(temp >= 3){
				for(k = 0; k < temp; k++){
					copy2[i + 10 * k] = '0';
				}
				if(print){
					printf("%d numbers are eliminated!\n", temp);
				}
				score += temp * temp;
			}
		}
	}
	// combine original and copied gameboard
	for(i = 0; i < 100; i++){
		if(copy1[i] == '0' || copy2[i] == '0'){
			gameBoard[i] = '0';
		}
	}
	return score;
}

//	parameter: x,y-coordinate, direction of combo, gameboard
//	change i, j by the direction of checking
//	check can_be_eleiminated combo on board with chosen dir (hor or ver)
//	check by recursion
//	return combo (if >3, return combo, else, return 1)
int isCombo(int x, int y, int dir, const char * gameBoard){
	int combo = 0;
	// (dir) 0:j=-1, 1:j=1, 2:i=-1, 3:i=1
	int i = (	 dir / 2) * (dir % 2 * 2 - 1);
	int j = (1 - dir / 2) * (dir % 2 * 2 - 1);
	
	if( boardElement(x, y, gameBoard) == boardElement(x+i, y+j, gameBoard) ){
		if( boardElement(x, y, gameBoard) != '@' && boardElement(x+i, y+j, gameBoard) != '@'){
			return isCombo(x+i, y+j, dir, gameBoard) + 1;
		}
	}
	return 1;
}

//	parameter: gameBoard
//	for every column, loop from buttom to top
//	sink all number and all 0 will be on top (stack: move all 0 upward)
//	update gameboard
//	return: none
void gravity(char * gameBoard){
	int index = 0;
	int i, j = 0;
	for(j = 0; j < 10; j++){
		index = 9;
		for(i = 9; i >= 0; i--){
			if(gameBoard[10 * i + j] != '0'){
				gameBoard[10 * index + j] = gameBoard[10 * i + j];
				index--;
			}
		}
		while(index >= 0){
			gameBoard[10 * index + j] = '0';
			index--;
		}
	}
}

//	parameter: gameBoard, K
//	loop from top to buttom, left to right
//	generate random rand() % K + 1
//	update board: change all 0 to random number
//	return: none 
void placeRandom(char * gameBoard, int K){
	int i = 0;
	for(i = 0; i < 100; i++){
		if(gameBoard[i] == '0'){
			gameBoard[i] = tinymt32_generate_uint32(&state) % K + '1';
		}
	}
}

//	parameter: iterations (step i), batch, score array for players, gameBoard, print index
//	print information:
//		1. Step ( 0 - 100 ) (and each process)
//		2. Total score
// 		3. Current board status by function printBoard
//	return: none
void printInfo(int iter, int batch, const int * score, const char * gameBoard, int print){
	if(print == 1){
		printf("############## STEP %d ###############\n", iter);
		printf("Total score ( Human Player ): %d\n", score[0]);
		printf("Total score ( AI Player )   : %d\n", score[1]);
	}else if(print == 2){
		printf("##### After Elimination %2d-%d #####\n", iter, batch);
	}else if(print == 3){
		printf("##### After Moving Down %2d-%d #####\n", iter, batch);
	}else if(print == 4){
		printf("##### After Generating %2d-%d ######\n", iter, batch);
	}
	if(print != 0){
		printBoard(gameBoard);
	}
}

void printBoard(const char * gameBoard){
	printf("    0 1 2 3 4 5 6 7 8 9\n");
	printf("   --------------------\n");
	int i = 0, j = 0;
	for(i = 0; i < 10; i++){
		printf("%d | ", i);
		for(j = 0; j < 10; j++){
			printf("%c ", gameBoard[10 * i + j]);
		}
		printf("\n");
	}
	printf("\n");
}
