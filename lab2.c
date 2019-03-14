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

//A Naive-Threaded algorithm
void basicTranspose(int twoD[SIZE][SIZE])
{

	int temp, i, j = 0;
	omp_set_num_threads(8);

#pragma omp parallel for lastprivate(i)
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

void diagonalTranspose(int twoD[SIZE][SIZE])
{

	int temp = 0;

	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < i + 1; j++)
		{

			if (i == j)
			{

				//printf("%i and %i ",i,j);
				//  printf("\n\n");

				for (int a = i; a < SIZE; a++)
				{

					temp = twoD[i][a];
					twoD[i][a] = twoD[a][i];
					twoD[a][i] = temp;
				}
			}
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
	int(*mat)[SIZE] = malloc(sizeof(int[SIZE][SIZE]));	//Two Dimensional Array

	printf("You entered %i as the size of the 2D matrix\n\n", SIZE);

	fill2D(twoD);
	print(twoD);

	printf("Now Transposing...\n\n");

	basicTranspose(twoD);
	print(twoD);

	//printf("\n\n\n\n");
	//fill2D(mat);
	//print(mat);

	//printf("Now Transposing AGAIN...\n\n");

	//diagonalTranspose(mat);
	// print(mat);

	return 0;
}
