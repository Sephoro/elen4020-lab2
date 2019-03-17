#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

int SIZE = 0;
int nextPosition = 0;
int numOfThreads = 8;

pthread_mutex_t syncing;

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




//Struct for Pthreads
struct threadInfo
{
	int pos;
	int size;
	int* Arr;
}; 


//Pthread Diagonal Transpose
void* pThread_diagonalTranspose(void* thread)
{
    
    
    struct threadInfo *tInfo = (struct threadInfo *) thread;
    int SIZE = tInfo->size;
    

   while(1)
   { 
             int temp;
				    for (int j = tInfo->pos+1; j < SIZE; ++j)
	          {

					  temp =  *(tInfo->Arr+ SIZE*tInfo->pos +j);
			      *(tInfo->Arr+ SIZE*tInfo->pos +j) = *(tInfo->Arr+ SIZE*j +tInfo->pos);
					  *(tInfo->Arr+ SIZE*j +tInfo->pos) = temp;
				
	          }
					
			       pthread_mutex_lock(&syncing);
      
            if(nextPosition < SIZE-1)
             {
            
                tInfo->pos = nextPosition;
                ++nextPosition;
            }else tInfo->pos = SIZE-1;
            pthread_mutex_unlock(&syncing);
            
            if (tInfo->pos==SIZE-1) break;
 
    }
      pthread_exit(NULL);
}


void DiagonalTransposePthread(int twoD[SIZE][SIZE])
{

	pthread_t threads[numOfThreads];
	struct threadInfo threadParameters[numOfThreads];
	nextPosition = numOfThreads;
	
	
	for(int i = 0; i < numOfThreads; i++)
	{
		threadParameters[i].size = SIZE;
		threadParameters[i].pos = i;
		threadParameters[i].Arr = twoD[0];
		

		pthread_create (&threads[i], NULL, &pThread_diagonalTranspose,&threadParameters[i]);	
	}

  	for(int i = 0; i < numOfThreads; i++)
	{
		pthread_join (threads[i], NULL);	
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
	timer(DiagonalTransposePthread,twoD,"Pthread Diagonal-Threading   ");
	

	printf("\n------------------------------------------------------------------");

	printf("\n\n");
	
  //Freeing the memory
	free(twoD);

	}
	

	return 0;
}
