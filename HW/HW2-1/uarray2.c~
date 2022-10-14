/*
* Authors:Andrew Crofts, Alex Tsuetaki
* Class: COMP40
* Homework2: iii
* filename: uarray2.c
* Date Modified: 9/27/19
* Purpose: implementation of the UArray2_T data structure
*
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "uarray.h"
#include "uarrayrep.h"
#include "mem.h"
#include "uarray2.h"


/*
* Name: UArray2_new
* Purpose: creates and allocates new 2D array, initializing values if height
*          and width and size
* Arguments: integer of widhth of 2D array, integer of height of 2D array, 
*            
* returns: the newly created UArray2_T 
*/
UArray2_T UArray2_new(int width_in, int height_in, unsigned int size_in){
    assert((width_in > 0) && (height_in > 0) && (size_in > 0)); 
    UArray2_T new_array2;  
    new_array2.height = height_in; 
    new_array2.width = width_in; 
    new_array2.size = size_in; 
    new_array2.uarray =  UArray_new((height_in*width_in), size_in); 
    return new_array2; 
}

/*
* Name: UArray2_at
* Purpose: gets and returns pointer to value at given coordinates
* Arguments: UArray2_T array to be interpreted and indexed, integer
*            x coordinate of index, integer y coordinate of index
* returns: void pointer to value at index
*/
void * UArray2_at(UArray2_T array_in, int width_in, int height_in){
    int index = (((height_in * array_in.width) + width_in));
    return UArray_at(array_in.uarray, index); 
}


/*
* Name: UArray2_free
* Purpose: frees the memory allocated for the 2D array
* Arguments: pointer to the 2D array to be freed and deleted
* returns: none
*/
void UArray2_free(UArray2_T *array_in){
    UArray_free(&(array_in->uarray));  
}


/*
* Name: Uarray2_map_col_major
* Purpose: iterates through each column and applies a function to the value at
*          at each index
* Arguments: UArray2_T array to be mapped through, apply function to be applied
*            at each value, client void pointer for misc use
* returns: none 
*/
void UArray2_map_col_major(UArray2_T array_in, void apply(int width, int height
, UArray2_T, void *value, void*cl) ,void *cl){    
    for(int i = 0; i < array_in.width; i++){
        for (int j = 0; j < array_in.height; j++){  
            void *val_at_idx = UArray2_at(array_in, i,j); 
            apply(i,j, array_in, val_at_idx, cl);
        }
    }

}


/*
* Name: UArray2_map_row_major
* Purpose: iterates through each row and applies a function to the value at
*          at each index
* Arguments: UArray2_T array to be mapped through, apply function to be applied
*            at each value, client void pointer for misc use
* returns: none
*/
void UArray2_map_row_major(UArray2_T array_in, void apply(int width, int height
, UArray2_T, void *value, void*cl), void *cl){
    for (int i = 0; i < array_in.height; i++){
        for (int k = 0; k < array_in.width; k++){
            void *val_at_idx = UArray2_at(array_in, k, i); 
            apply(k, i, array_in, val_at_idx, cl); 
        }

    } 

}


/*
* Name: UArray2_width
* Purpose: returns the width of the 2D array
* Arguments: 2D array to be analyzed
* returns: integer representing the width of given array
*/
int UArray2_width(UArray2_T array_in){
    int temp_width = array_in.width; 
    return temp_width;
}


/*
* Name: UArray2_height
* Purpose: returns the height of the 2D array
* Arguments: 2D array to be analyzed
* returns: integer representing height
*/
int UArray2_height(UArray2_T array_in){
    int temp_height = array_in.height; 
    return temp_height;

}


/*
* Name: UArray2_size
* Purpose: returns the size of the input array 
* Arguments: 2D array to be analyzed
* returns: integer representing size of each value
*/
int UArray2_size(UArray2_T array_in){
    int temp_size = array_in.size;
    return temp_size;

}
