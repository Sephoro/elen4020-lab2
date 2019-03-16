#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int SIZE = 0;

//generate random nxn matrix
void fill2D(int twoD[SIZE][SIZE])
{

	int num = 0;

	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < SIZE; j++)
		{

			num = rand() % 20;
			twoD[i][j] = num;
		}
	}
}

//display matrix
void print(int twoD[SIZE][SIZE])
{

	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < SIZE; j++)
		{

			printf("%i ", twoD[i][j]);
		}

		printf("\n");
	}

	printf("\n");
}

// Basic Naive Transpose
void NaiveTransposeBasic(int twoD[SIZE][SIZE])
{

	int temp = 0;

	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < i; j++)
		{

			temp = twoD[i][j];
			twoD[i][j] = twoD[j][i];
			twoD[j][i] = temp;
		}
	}
}

// Basic Naive Transpose
void NaiveTransposeOpenMP(int twoD[SIZE][SIZE])
{

	int temp = 0;
	omp_set_num_threads(8);

    #pragma omp parallel for
	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < i; j++)
		{

			temp = twoD[i][j];
			twoD[i][j] = twoD[j][i];
			twoD[j][i] = temp;
		}
	}
}

//Basic Diagonal Transpose
void DiagonalTransposeBasic(int twoD[SIZE][SIZE])
{

	int temp = 0;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = i + 1; j < SIZE; j++)
		{

			temp = twoD[i][j];
			twoD[i][j] = twoD[j][i];
			twoD[j][i] = temp;
		}
	}
}

//OpenMP Diagonal Transpose
void DiagonalTransposeOpenMP(int twoD[SIZE][SIZE])
{

	int temp = 0;
	omp_set_num_threads(8);

    #pragma omp parallel for
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = i + 1; j < SIZE; j++)
		{

			temp = twoD[i][j];
			twoD[i][j] = twoD[j][i];
			twoD[j][i] = temp;
		}
	}
}



int main()
{

	//generate seed
	srand(time(0));

	printf("Please input size: ");
	scanf("%i", &SIZE);

	int(*twoD)[SIZE] = malloc(sizeof(int[SIZE][SIZE])); //Two Dimensional Array
	
	printf("You entered %i as the size of the 2D matrix\n\n", SIZE);

	fill2D(twoD);
	print(twoD);

	printf("Naive basic...\n");
	NaiveTransposeBasic(twoD);
	print(twoD);

	printf("\n");
	
	printf("Naive OPM...\n");
	NaiveTransposeOpenMP(twoD);
	print(twoD);

	printf("\n");

	printf("Diagonal basic...\n");
	DiagonalTransposeBasic(twoD);
	print(twoD);

    printf("\n");
    printf("Diagonal OPM...\n");
	DiagonalTransposeOpenMP(twoD);
	print(twoD);

	return 0;
}
