#include "basic_1155157516.h"
#include "tinymt32.h"

int gameBoard[100] = {0};
int totalScore = 0;
tinymt32_t state;

int main(){
	//int i= 0, j = 0, temp = 0;
	uint32_t pesudo_seed = 0;
	tinymt32_init(&state, pesudo_seed);
	int currentInput = 0;
	int numOfIterations = 0;
	int stepScore = 0;
	int score = 0;
	// 	init
	initBoard();
	// 	for every step:
	while( numOfIterations < 100 ){
		printInfo(numOfIterations);
		stepScore = 0;
		currentInput = getInput();
		swap( currentInput );
		
		do{
			score = eliminate();
			if(score != 0){
				stepScore += score;
				gravity();
				placeRandom();
				//printInfo(numOfIterations);
			}
		}while(score != 0);
		printf("Score obtained in this step: %d\n", stepScore);
		totalScore += stepScore;
		numOfIterations++;
	}
	//	terminate
	printf("total score : %d\n", totalScore);
	printf("mean score : %lf", (double)totalScore);
	
	return 0;
}

//	parameter: none
//  initialize gameBoard hard-codingly with K = 4
//	value of gameBoard will be updated
//	return value: none
void initBoard(){
	int temp[105] = {	1, 2, 3, 4, 3, 2, 3, 2, 3, 4,
						2, 3, 4, 2, 3, 4, 1, 4, 1, 1, 
						4, 2, 3, 2, 1, 1, 4, 1, 4, 1,
						2, 3, 4, 3, 3, 4, 1, 2, 1, 4,
						4, 3, 3, 2, 3, 1, 2, 3, 2, 2,
						2, 1, 4, 3, 4, 1, 4, 1, 2, 4,
						2, 1, 1, 2, 1, 4, 2, 4, 3, 3,
						4, 2, 4, 3, 3, 1, 2, 1, 1, 3,
						2, 1, 1, 4, 4, 2, 1, 1, 4, 1,
						1, 3, 4, 2, 1, 2, 4, 2, 1, 3,
					};
	int i = 0;
	for(i = 0; i < 100; i++){
		gameBoard[i] = temp[i];
	}
}

// 	parameter: none
// 	get input from user
//	keep asking input intil it gets a valid input
//	check input using isValidInput(input);
//	return value: validated input
int getInput(){
	int input = 0, cnt = 0;
	do{
		if(cnt != 0){
			printf("Invalid input. Try again.\n");
		}
		printf("Please enter swapping locations:");
		scanf("%d", &input);
		cnt++;
	}while( !isValidInput(input) );
	return input;
}

//	parameter: input from getInput (not validate yet)
// 	check whether the input is valid for:
// 	1. input in range 0000 - 9999
//	2. 2 numbers is adjacent to each others
//	return value: 1(true) for valid, 0(false) for invalid
int isValidInput(int input){
	if(input < 0 && input > 9999){
		return 0;
	}else{
		int x1, y1, x2, y2;
		x1 = input / 1000;
		y1 = (input / 100) % 10;
		x2 = (input / 10) % 10;
		y2 = input % 10;
		if( abs(x1 - x2) + abs(y1 - y2) == 1){
			return 1;
		}else{
			return 0;
		}
	}
}

// 	parameter: validated input number, e.g. 1112
// 	task: swap elements in gameBoard, e.g. 11 and 12
//	end by print info
//	return value: none
void swap(int input){
	int num1, num2;
	num1 = input / 100;
	num2 = input % 100;
	
	int temp = 0;
	temp = gameBoard[num1];
	gameBoard[num1] = gameBoard[num2];
	gameBoard[num2] = temp;
	
	printf("Numbers at %d and %d are swapped.\n", num1, num2);
}

//	parameter: x, y coordinate of gameBoard to be checked, copied array of gameboard
//	check whether there is horizonal combo starting from the given coordinate
//	i.e. 3333XX (horizontal) ~similar to isVerCombo
//	if combo exists, return number of combo [3-5]
// 	if combo DNE (1-2), return -1
int isHorCombo(int x, int y, int board[]){
	int consecutive = 1;
	int num = board[10 * x + y];
	int i = 0;
	for(i = y+1; i <= 9; i++){
		if(num != board[10 * x + i]){
			if(consecutive >= 3){
				return consecutive;
			}else{
				return -1;
			}
		}else{
			consecutive++;
		}
	}
	if(consecutive >= 3){
		return consecutive;
	}else{
		return -1;
	}
}

//	parameter: x, y coordinate of gameBoard to be checked, copied array of gameboard
//	check whether there is vertical combo starting from the given coordinate
//	i.e. 3333XX (vertical) ~similar to isHorCombo
//	if combo exists, return number of combo [3-5]
// 	if combo DNE (1-2), return -1
int isVerCombo(int x, int y, int board[]){
	int consecutive = 1;
	int num = board[10 * x + y];
	int i = 0;
	for(i = x+1; i <= 9; i++){
		if(num != board[10 * i + y]){
			if(consecutive >= 3){
				return consecutive;
			}else{
				return -1;
			}
		}else{
			consecutive++;
		}
	}
	if(consecutive >= 3){
		return consecutive;
	}else{
		return -1;
	}
}

//	parameter: none
//	Given a gameBoard, check combo by isHorCombo and isVerCombo
//	if combo exists, add score + change element to 0 (eliminated)
//		1. create copied gameBoard
//		2. modify content of copy1 (for hor) and copy2 (for ver) gameboard
//		3. eliminate combo and print info
//		4. combine modification and make changes to original gameboard
//	return score of this round (partly, for the 1st batch)
int eliminate(){
	int i, j, k = 0;
	int temp1, temp2 = 0;
	int score = 0;
	// make a copy of gameBoard
	int copy1[100], copy2[100];
	for(i = 0; i < 100; i++){
		copy1[i] = gameBoard[i];
		copy2[i] = gameBoard[i];
	}
	// check horizontal and vertical combo
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			// if horizontal combo, change copied gameboard 1 to 0
			if(copy1[10 * i + j] != 0){
				temp1 = isHorCombo(i, j, copy1);
				if(temp1 != -1){
					for(k = 0; k < temp1; k++){
						copy1[i * 10 + j + k] = 0;
					}
					printf("%d numbers are eliminated!\n", temp1);
					score += temp1 * temp1;
				}
			}
			// if vertical combo, change copied gameboard 2 to 0
			if(copy2[10 * i + j] != 0){
				temp2 = isVerCombo(i, j, copy2);
				if(temp2 != -1){
					for(k = 0; k < temp2; k++){
						copy2[(i + k) * 10 + j] = 0;
					}
					printf("%d numbers are eliminated!\n", temp2);
					score += temp2 * temp2;
				}
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

//	parameter: none
//	for every column, loop from buttom to top
//	sink all number and all 0 will be on top
//	update gameboard
//	return: none
void gravity(){
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

//	parameter: none
//	loop from top to buttom, left to right
//	generate random by init srand(0), then rand() % 4 + 1
//	update board: change all 0 to random number
//	return: none 
void placeRandom(){
	//srand(0);
	int i = 0;
	for(i = 0; i < 100; i++){
		if(gameBoard[i] == 0){
			gameBoard[i] = tinymt32_generate_uint32(&state) % 4 + 1;
		}
	}
}

//	parameter: iterations (step i)
//	print information:
//		1. Step ( 0 - 100 )
//		2. Total score
// 		3. Current board status
//	return: none
void printInfo(int iterations){
	// step
	printf("############## STEP %d ###############\n", iterations);
	// total score
	printf("Total score: %d\n", totalScore);
	
	// print grid
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

