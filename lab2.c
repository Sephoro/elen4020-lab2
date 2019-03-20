#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

int SIZE = 128;
int BLOCK_SIZE = 16;
int NEXT_POS = 0;
int NUM_OF_THREADS = 8;

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


void *swap(int *a, int *b){

   int temp = *a;
   *a = *b;
   *b = temp;
 }


// Basic Naive Transpose
void NaiveTransposeBasic(int twoD[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < i; j++)
		{
                        swap(&twoD[i][j], &twoD[j][i]);
	
		}
	}
}

// Naive Transpose Open MP
void NaiveTransposeOpenMP(int twoD[SIZE][SIZE])
{


omp_set_num_threads(NUM_OF_THREADS);

    #pragma omp parallel 
    {
	  for (int i = 0; i < SIZE; i++)
	  {
                #pragma omp for nowait
		for (int j = 0; j < i; j++)
		{
                     swap(&twoD[i][j], &twoD[j][i]);
		}
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
                    swap(&twoD[i][j], &twoD[j][i]);
		}
	}
}

//OpenMP Diagonal Transpose
void DiagonalTransposeOpenMP(int twoD[SIZE][SIZE])
{

	int temp = 0;
	omp_set_num_threads(NUM_OF_THREADS);

    #pragma omp parallel
	{
    
	for (int i = 0; i < SIZE; i++)
	{
		#pragma omp for nowait
		for (int j = i + 1; j < SIZE; j++)
		{
                    swap(&twoD[i][j], &twoD[j][i]);
		}
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

		while (1)
		{
			int temp;
			for (int j = tInfo->pos + 1; j < SIZE; ++j)
			{

				temp = *(tInfo->Arr + SIZE * tInfo->pos + j);
				*(tInfo->Arr + SIZE * tInfo->pos + j) = *(tInfo->Arr + SIZE * j + tInfo->pos);
				*(tInfo->Arr + SIZE * j + tInfo->pos) = temp;
			}

			pthread_mutex_lock(&syncing);

			if (NEXT_POS < SIZE - 1)
			{

				tInfo->pos = NEXT_POS;
				++NEXT_POS;
			}
			else
				tInfo->pos = SIZE - 1;
			pthread_mutex_unlock(&syncing);

			if (tInfo->pos == SIZE - 1)
				break;
		}
		pthread_exit(NULL);
}


void DiagonalTransposePthread(int twoD[SIZE][SIZE])
{

	pthread_t threads[NUM_OF_THREADS];
	struct threadInfo threadParameters[NUM_OF_THREADS];
	NEXT_POS = NUM_OF_THREADS;
	
	
	for(int i = 0; i < NUM_OF_THREADS; i++)
	{
		threadParameters[i].size = SIZE;
		threadParameters[i].pos = i;
		threadParameters[i].Arr = twoD[0];
		

		pthread_create (&threads[i], NULL, &pThread_diagonalTranspose,&threadParameters[i]);	
	}

  	for(int i = 0; i < NUM_OF_THREADS; i++)
	{
		pthread_join (threads[i], NULL);	
	}
	

} 

/** WARNING: BLOCK TRANSPOSITION AHEAD!! **/

//transposing of each element in the block
void blockElementTranspose(int twoD[SIZE][SIZE]){

   int otherRow = 0;
   int otherCol = 0;


   for(int i = 0; i < SIZE; i+=BLOCK_SIZE){
   
     for(int j= 0; j < SIZE; j+=BLOCK_SIZE){
     
          for(int k = 0; k < BLOCK_SIZE; k++){
	  
	       for(int l = 0; l < k; l++){
	          
                    if(i!=j){

                            otherRow =  (i + l);
                            otherCol =  (j + k);                       
                             
                    }
                    else{
                    
                           otherRow = (l + j);
                           otherCol = (k + i);
                    }

	            swap(&twoD[k+i][l+j],&twoD[otherRow][otherCol]);
                
	          }
	  
	    }
       }
   
   }

}

//Swapping of blocks
void blockSwap(int i, int j, int *arr){


   int posA = i*SIZE + j;
   int posB = j*SIZE + i;
   
   //B array? ;)
   int *brr = arr;
    arr+=posA;
    brr+=posB;

   for(int i = 0; i < BLOCK_SIZE; i++){
        
        for(int j =0; j < BLOCK_SIZE; j++){
           
            swap(arr,brr);
            
            arr++;
            brr++;
          }
       
        arr = arr - BLOCK_SIZE + SIZE;
        brr = brr - BLOCK_SIZE + SIZE;
      }

}

//Where blocks are transposed
void blockTranspose(int twoD[SIZE][SIZE]){
  
   for(int i =0; i < SIZE;i+=BLOCK_SIZE){
      
      
      for(int j =0; j < i; j+=BLOCK_SIZE)
            {

               blockSwap(i,j, twoD[0]);

            }

    }
}

//Basic Block Transposition
void BlockTransposeBasic(int twoD[SIZE][SIZE]){

    blockElementTranspose(twoD);
    blockTranspose(twoD);
 }


/** PTHREADS BLOCK TRANSPOSITION AHEAD **/

//Below are the pthread Block Transposition variables

struct blockThreadInfo{

   int row;
	 int *arr;

};

int NEXT_ROW_E = 0;
int NEXT_ROW_B = 0;
pthread_mutex_t stillBusy; 

// pthread version of BlockElement Transpose

void* PthreadsBlockElementTranspose( void* blockInfo){


  struct blockThreadInfo *blockThread = (struct blockThreadInfo*) blockInfo;
   
  while(1){

   	int otherRow = 0;
   	int otherCol = 0;

   	for(int i = blockThread->row; i < (blockThread->row+BLOCK_SIZE); i+=BLOCK_SIZE){
   
     		for(int j=0; j < SIZE ; j+=BLOCK_SIZE){
     
        		for(int k = 0; k < BLOCK_SIZE; k++){
	  
	       			for(int l = 0; l < k; l++){
	          
                  		 	if(i!=j){
                               
	                               	         //Not on diagonal
                      		     	         otherRow =  (i + l);
                          		  	 otherCol =  (j + k);                       
                    					}
                    			else{
                             			//Diagonal
                        		 	otherRow = (l + j);
                           			otherCol = (k + i);
                    					}
 
                     			swap((blockThread->arr+ (k+i)*SIZE + (l+j)), (blockThread->arr + (otherRow)*SIZE + (otherCol)));
              				     
				 }
			 }
      		   }
	   }

 

    //Done?
   if(NEXT_ROW_E >= SIZE - 1) break;
     
    //Controlled Entrance

     pthread_mutex_lock(&stillBusy);
    
     		blockThread->row = NEXT_ROW_E;
     		NEXT_ROW_E+=BLOCK_SIZE;

      pthread_mutex_unlock(&stillBusy);
   
   }

   pthread_exit(NULL);

}

//Pthread version of block Transpose

void* PthreadsblockTranspose(void* blockInfo){


  struct blockThreadInfo *blockThread = (struct blockThreadInfo*) blockInfo;
  
	while(1){
   
	 	for(int i = blockThread->row; i < (blockThread->row+BLOCK_SIZE); i+=BLOCK_SIZE){
      
      			for(int j = 0; j < i; j+=BLOCK_SIZE)
            		 {
			     blockSwap(i,j, blockThread->arr);
								 
			 }
		}

         	
	     //Finished?
   	    if(NEXT_ROW_B >= SIZE - 1) break;
  
	    // Controlled Access

  	     pthread_mutex_lock(&stillBusy);
    
   		 blockThread->row = NEXT_ROW_B;
     		 NEXT_ROW_B+=BLOCK_SIZE;

   	     pthread_mutex_unlock(&stillBusy);
 				
		}

   pthread_exit(NULL);
}

// Full Pthread Block Transposition

void BlockTransposePthread(int twoD[SIZE][SIZE]){
        
   //For Element transposition

   pthread_t eThreads[NUM_OF_THREADS];
   struct blockThreadInfo eBlockThreads[NUM_OF_THREADS];
   
	 //Assuming size of the matrix will always be greater than 8
    NEXT_ROW_E = NUM_OF_THREADS*BLOCK_SIZE;
   // SIZE >> 8

   for(int i = 0; i < NUM_OF_THREADS; i++){
       
        eBlockThreads[i].row = i*BLOCK_SIZE;
        eBlockThreads[i].arr = twoD[0];
        pthread_create(&eThreads[i], NULL, &PthreadsBlockElementTranspose, &eBlockThreads[i]);
        

    }

   	for(int i =0; i <NUM_OF_THREADS; i++){

              pthread_join(eThreads[i],NULL);
    
           }
   

	 //For Transposing the blocks

   	pthread_t bThreads[NUM_OF_THREADS];
   	struct blockThreadInfo blockThreads[NUM_OF_THREADS];
   	NEXT_ROW_B = BLOCK_SIZE*NUM_OF_THREADS;

   
	 for(int i = 0; i < NUM_OF_THREADS; i++){
       
        	blockThreads[i].arr = twoD[0];
       		blockThreads[i].row = i*BLOCK_SIZE;
        	pthread_create(&bThreads[i], NULL, &PthreadsblockTranspose, &blockThreads[i]);

    		}
    
      	for(int i =0; i <NUM_OF_THREADS; i++){

              	pthread_join(bThreads[i],NULL);
    
           	}

}




//A timer function that accepts functions
void timer(void(*f)(int(*twoD)[(int)(SIZE)]), int twoD[SIZE][SIZE], char *type_transpose){

	struct timeval start, end;
    
	//Start timer
	gettimeofday( &start, NULL );

           f(twoD);
        
        //end timer
        gettimeofday( &end, NULL );
	
	double duration = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
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
	
        //Naive
	timer(NaiveTransposeBasic,twoD,"Basic Naive-Transposition    ");
        timer(NaiveTransposeOpenMP,twoD,"OpenMP Naive-Threading       ");
	//Diagonal
        timer(DiagonalTransposeBasic,twoD,"Basic Diagonal-Transposition ");
	timer(DiagonalTransposeOpenMP,twoD,"OpenMP Diagonal-Threading    ");
	timer(DiagonalTransposePthread,twoD,"Pthread Diagonal-Threading   ");
        //Block
        timer(BlockTransposeBasic,twoD,"Basic Block-Transposition    ");
	timer(BlockTransposePthread,twoD,"Pthread Block-Transposition  ");
      
        printf("\n------------------------------------------------------------------");

	printf("\n\n");

        //Freeing the memory
	free(twoD);

	}
	

	return 0;
}

