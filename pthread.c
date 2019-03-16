#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>




int nextPosition = 0;
int SIZE = 0;
int numOfThreads = 8;

pthread_mutex_t syncing;

struct threadInfo
{
	int pos;
	int size;
	int* Arr;
}; 





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

int main()
{

	//generate seed
  
	printf("Please input size: ");
	scanf("%i", &SIZE);

	
	  int newSize = SIZE*SIZE;
	  int add = 0;
    int *mat = (int *) malloc(newSize*sizeof(int));
    
    for (int i=0;i<SIZE;i++)
   {
      for(int j=0;j<SIZE;j++){
        *(mat+ SIZE*i +j) = add;
        add++;
      }
    }
	
		 //PRINTING MATRIX
	 
	 	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < SIZE; j++)
		{

			printf("%i ", *(mat+ SIZE*i +j));
		}

		printf("\n");
	}

	printf("\n");
	 
	
	

	printf("NEW TRANSPOSED MATRIX");
  printf("\n\n\n");
	
	
	
	pthread_t threads[numOfThreads];
	struct threadInfo threadParameters[numOfThreads];
	nextPosition = numOfThreads;
	
	
	for(int i = 0; i < numOfThreads; i++)
	{
		threadParameters[i].size = SIZE;
		threadParameters[i].pos = i;
		threadParameters[i].Arr = mat;
		

		pthread_create (&threads[i], NULL, &pThread_diagonalTranspose,&threadParameters[i]);	
	}

  	for(int i = 0; i < numOfThreads; i++)
	{
		pthread_join (threads[i], NULL);	
	}

	

	 //PRINTING MATRIX
	 
	 	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < SIZE; j++)
		{

			printf("%i ", *(mat+ SIZE*i +j));
		}

		printf("\n");
	}

	printf("\n");
	 

	return 0;
}