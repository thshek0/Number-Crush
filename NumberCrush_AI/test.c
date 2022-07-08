#include "aiplayer_1155157516.h"

void quickSort(int[][10], int, int);
int partition (int[][10], int, int);
void sortSwap(int*, int*);

int test( int k, const int * board ){
	
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
	
	for(i = 0; i < size; i++){
		for(j = 0; j < 50; j++){
			testBoard = duplicateBoard_1155157516(board);
			simWithRan_1155157516(testBoard, posEffSwap, i, k);
		}
	}
	quickSort(posEffSwap, 0, size - 1);
	
	
	for(i = 0; i < size; i++){
		for(j = 0; j <= 8; j++){
			printf("%d ", posEffSwap[i][j]);
		}
		printf("\n");
	}
	
	return dumbAI1_1155157516(posEffSwap, size, 6);
}

void sortSwap(int* a, int* b){ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 
/* This function takes last element as pivot, places 
the pivot element at its correct position in sorted 
	array, and places all smaller (smaller than pivot) 
to left of pivot and all greater elements to right 
of pivot */
int partition (int arr[][10], int low, int high){ 
	int pivot = arr[high][6]; // pivot 
	int i = (low - 1); // Index of smaller element 
	int j = low, k = 0;
	for (j = low; j <= high- 1; j++){ 
		// If current element is smaller than the pivot 
		if (arr[j][6] < pivot){ 
			i++; // increment index of smaller element 
			for(k = 0; k <=8; k++){
				sortSwap(&arr[i][k], &arr[j][k]);
			}
		} 
	} 
	for(k = 0; k <= 8; k++){
		sortSwap(&arr[i + 1][k], &arr[high][k]); 
	}
	return i + 1; 
} 

/* The main function that implements QuickSort 
arr[] --> Array to be sorted, 
low --> Starting index, 
high --> Ending index */
void quickSort(int arr[][10], int low, int high){ 
	if (low < high)	{ 
		/* pi is partitioning index, arr[p] is now 
		at right place */
		int pi = partition(arr, low, high); 

		// Separately sort elements before 
		// partition and after partition 
		quickSort(arr, low, pi - 1); 
		quickSort(arr, pi + 1, high); 
	} 
} 

void simulate(int * testBoard, int posEffSwap[][10], int index, int k, int loops){
	int size = getEffSwapList_1155157516(testBoard, posEffSwap);
	int currentScore[100][10] = {0};
	int i, j;
	
	for(i = 0; i < size; i++){
		for(j = 0; j < 50; j++){
			testBoard = duplicateBoard_1155157516(testBoard);
			simWithRan_1155157516(testBoard, currentScore, i, k);
			//simulate(testBoard)
		}
	}
	quickSort(posEffSwap, 0, size - 1);
	
}
