/*uarray2b.c
 *implimentation of a second version of uarray2 which uses blocking
 *Author: Alex Tsuetaki, Gabe McNinch
 */
#include <mem.h>
#include <string.h>
#include <uarray2.h>
#include "a2blocked.h"
#include <stdio.h>

typedef struct UArray2b_T{
	int width;
	int height;
	int size;
	int blocksize;
	UArray2_T arr2d;
}*T;

T UArray2b_new(int width, int height, int size, int blocksize){
	T array;
	NEW(array);
	array->width = width;
	array->height = height;
	array->size = size;
	array->blocksize = blocksize;
	array->arr2d = UArray2_new((width/blocksize+1)*(height/blocksize+1),
				   blocksize*blocksize,size);
	return array;
}

T UArray2b_new_64K_block(int width, int height, int size){
        T arr;
	NEW(arr);
	(*arr).width = width;
	(*arr).height = height;
	(*arr).size = size;
	(*arr).blocksize = 256;
	(*arr).arr2d = UArray2_new((width/(*arr).blocksize+1)*
				   (height/(*arr).blocksize+1), 
				   (*arr).blocksize/size*(*arr).blocksize/size, 
				   size);
	return arr;
}

void UArray2b_free(T * arr){
	UArray2_free(&((*arr)->arr2d));
}
int UArray2b_width(T arr){
	return arr->width;
}
int UArray2b_height(T arr){
	return arr->height;
}
int UArray2b_size(T arr){
	return arr->size;
}
int UArray2b_blocksize(T arr){
	return arr->blocksize;
}


void * UArray2b_at(T arr, int i, int j){
	//printf("[%d,%d]: ",i,j);
	void *temp =  UArray2_at(arr->arr2d,
			  i/arr->blocksize *(arr->width/arr->blocksize+1) 
			  + j/arr->blocksize,
			  ((i % arr->blocksize)*arr->blocksize) 
			  + (j % arr->blocksize));
	//int *inttemp = temp;
	//printf("%d\n",*inttemp);
	return temp;
}

void UArray2b_map(T arr, void apply(int col, int row, T array,
				    void* elem, void *cl),void *cl){
   int wend = arr->blocksize;
   int hend = arr->blocksize;
   for(int i = 0; i < arr->width/(arr->blocksize);i++){
       if(arr->blocksize*(i+1) > arr->width){
	       wend = arr->width%(arr->blocksize);
       }
       for(int j = 0; j< arr->height/(arr->blocksize);j++){
	   if(arr->blocksize*(j+1) > arr->height){
		   hend = arr->height%(arr->blocksize);
	   }
	   for(int k = 0; k< wend;k++){
	       for(int l = 0; l< hend;l++){
		   apply(i+k,j+l,arr,UArray2b_at(arr,i*
		      (arr->width/arr->blocksize +1)+k,j*arr->blocksize+l),cl);
	       }	   
	   }
	   hend = arr->blocksize;
       }
       wend = arr->blocksize;
   }
}
