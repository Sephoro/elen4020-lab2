#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int SIZE=0;

//generate random nxn matrix
void fill2D(int twoD[SIZE][SIZE]){
   
   int num = 0;

   for(int i =0; i < SIZE; i++){
   
      for(int j =0; j < SIZE; j++){
          
          num = rand()%20; 
          twoD[i][j] = num; 
      
         }
   
     }

  }


//display matrix
void print(int twoD[SIZE][SIZE]){

    for(int i = 0; i <SIZE; i++){
    
	    for(int j = 0; j <SIZE; j++){
	    	
		    printf("%i ", twoD[i][j]);
	    
	    }
	
	    printf("\n");
    
    }

    printf("\n");
}


void basicTranspose(int twoD[SIZE][SIZE]){

 int temp =0;

 for(int i=0; i < SIZE; i++){
 
 	for(int j =0; j < i; j++){
	       
	        temp = twoD[i][j];
		twoD[i][j] = twoD[j][i];
		twoD[j][i] =temp;
	 
	}
   }

}


int main(){
 
 //generate seed
 srand(time(0));

 printf("Please input size: ");
 scanf("%i", &SIZE);

 int (*twoD)[SIZE] = malloc(sizeof(int[SIZE][SIZE])); //Two Dimensional Array
 

 printf("You entered %i as the size of the 2D matrix\n\n", SIZE);

 fill2D(twoD);
 print(twoD);

 printf("Now Transposing...\n\n");

 basicTranspose(twoD);
 print(twoD);

 return 0;
 }

