#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include <string.h>

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

//A timer function that accepts functions
void timer(void(*f)(int(*twoD)[(int)(SIZE)]), int twoD[SIZE][SIZE], char *type_transpose){

	struct timeval start, end;
    
	//Start timer
	gettimeofday( &start, NULL );
    f(twoD);
    gettimeofday( &end, NULL );
	//end timer
    fill2D(twoD);

	double duration = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);fill2D(twoD);
    printf("%s  t = %g milliseconds\n",type_transpose,  duration*1e3 );

}

int main()
{

	//generate seed
	srand(time(0));
	 
	//matrix sizes
	int array[4] = {128, 1024, 2048, 4096};

	for(int i = 0; i<4; i++){

	SIZE = array[i];
	int(*twoD)[SIZE] = malloc(sizeof(int[SIZE][SIZE])); 
	fill2D(twoD);
	printf("---------------------------------%i----------------------------- \n\n", SIZE);
	
	timer(NaiveTransposeBasic,twoD,"Basic Naive-Transposition    ");
	timer(DiagonalTransposeBasic,twoD,"Basic Diagonal-Transposition ");
	
	timer(NaiveTransposeOpenMP,twoD,"OpenMP Naive-Threading       ");
	timer(DiagonalTransposeOpenMP,twoD,"OpenMP Diagonal-Threading    ");

	printf("\n------------------------------------------------------------------");

	printf("\n\n");

	free(twoD);

	}
	

	return 0;
}
