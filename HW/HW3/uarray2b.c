/*uarray2b.c
 *implimentation of a second version of uarray2 which uses blocking
 *Author: Alex Tsuetaki, Gabe McNinch
 */
#include <mem.h>
#include <string.h>
#include <uarray2.h>
#include <uarray.h>
#include "a2blocked.h"
#include <stdio.h>
#include <assert.h>

const int DEFAULT_BLOCKSIZE = 256;
/*
 *Struct UArray2b_T
 *int width: the width of the array
 *int height: the height of the array
 *int size: the size of one element in the array
 *int blocksize: the amount of elements on one side a block
 *UArray2_T: a uarray2 which the values in UArray2b_T stores
 *           its values in
 *purpose: represents the blocked 2d array
 */
typedef struct UArray2b_T{
        int width;
        int height;
        int size;
        int blocksize;
        UArray_T arr2d;
}*T;

/*
 *UArray2b_new
 *purpose: to create a new UArray2b_T object with a user designated block
 *         size
 *argument: int width: the width of the array that is to be created
 *          int height: the height of the arrary that is to be created
 *          int size: the size of the elements to be stored in the array
 *          int blocksize: the size of the sides of the quare blocks which
 *                         make up UArray2b_T
 *
 *returns: a UArray2b_T *  (aka a pointer to a UArray2b_T object).
 */
T UArray2b_new(int width, int height, int size, int blocksize){
        T array;
        NEW(array);
        array->width = width;
        array->height = height;
        array->size = size;
        array->blocksize = blocksize;
        array->arr2d = UArray_new((width/blocksize+1)*(height/blocksize+1),
                              sizeof(UArray2_T));

    for (int i = 0; i < (width/blocksize+1)*(height/blocksize+1); i++) {
        UArray2_T *rowp = UArray_at(array->arr2d, i);
        *rowp = UArray2_new(blocksize, blocksize, size);
    }
        return array;
}

/*
 *UArray2b_new
 *purpose: to create a new UArray2b_T object with a block size of 256
 *         aka a 64k (64*1024)block, a block contaning 64*1024 elements
 *argument: int width: the width of the array that is to be created
 *          int height: the height of the arrary that is to be created
 *          int size: the size of the elements to be stored in the array
 *
 *returns: a UArray2b_T *  (aka a pointer to a UArray2b_T object).
 */
T UArray2b_new_64K_block(int width, int height, int size){
    T arr;
    NEW(arr);
        (*arr).width = width;
        (*arr).height = height;
        (*arr).size = size;
        (*arr).blocksize = DEFAULT_BLOCKSIZE;
        (*arr).arr2d = UArray_new(
        (width/DEFAULT_BLOCKSIZE+1)*(height/DEFAULT_BLOCKSIZE+1),
        sizeof(UArray2_T));
    int blocksize = DEFAULT_BLOCKSIZE;
    for (int i = 0; i < (width/blocksize+1)*(height/blocksize+1); i++) {
        UArray2_T *rowp = UArray_at(arr->arr2d, i);
        *rowp = UArray2_new(blocksize, blocksize, size);
    }


        return arr;
}

/*
 *UArray2b_free
 *purpose: to free the data realted to a UArray2_T object
 *arguments: the pointer to a UArray2_T pointer to be freed
 *returns: none
 */
void UArray2b_free(T * arr){
        UArray_free(&(*arr)->arr2d);
    FREE(*arr);
}

/*
 *UArray2b_width
 *purpose: to get the width of an 2darray
 *arguments: a pointer to a UArray2b_T object
 *return: the width of the 2darray
 */
int UArray2b_width(T arr){
        return arr->width;
}

/*
 *UArray2b_height
 *purpose: to get the height of an 2darray
 *arguments: a pointer to a UArray2b_T object
 *returns: the height of the 2darray
 */
int UArray2b_height(T arr){
        return arr->height;
}

/*
 *UArray2b_size
 *purpose: to get the size of an element in a 2darray
 *arguments: a pointer to a UArray2b_T object
 *returns: the size of an element in a 2darray
 */
int UArray2b_size(T arr){
        return arr->size;
}

/*
 *UArray2b_blocksize
 *purpose: to get the size of a block in a 2darrat
 *arguments: a pointer to a UArrat2b_T object
 *returns: the size of one side of a block
 */
int UArray2b_blocksize(T arr){
        return arr->blocksize;
}

/*
 *UArray2b_at
 *purpose: to get the value at a point in the array
 *arguments: T arr: a pointer to a UArray2b_T object
 *            int i: the x value of the element to be returned
 *            int j: the y value of the elemnent to be returned
 *returns: the value at point i,j
 */
void * UArray2b_at(T arr, int i, int j){
    int w = arr->width;
    int b = arr->blocksize;
    

        return UArray2_at(*(UArray2_T *)(UArray_at(arr->arr2d,
                            (j/b)*(w/b+1)+i/b)),
                            i % b, j % b);
}

/*
 *UArray2b_map
 *purpose: to map over the function
 *arguments: UArray2b_T arr: a pointer to an array
 *           void apply: a function to be applied
 *           void cl: a closer to be used when in apply
 *returns: none
 */
void UArray2b_map(T arr, void apply(int col, int row, T array,
                                    void* elem, void *cl),void *cl){
    int h = arr->height;
    int w = arr->width;
    int b = arr->blocksize;
    int blpr = w/b + 1; /* blocks per row */
        int wend = b;
        int hend = b;
    
    for(int i = 0; i < (blpr) * (h/b + 1); i++){
        if(i / (blpr) == (h/b)){
            hend = h % b;
        }
        if((i+1) % (blpr) == 0){
            wend = w % b;
        }
        for (int l = 0; l < hend; l++) {
            for(int k = 0; k < wend; k++){
                
                apply(i % (blpr) * b + k, i / (blpr) * b + l, 
                      arr, UArray2b_at(arr, i % (blpr) * b + k,
                                       i / (blpr) * b + l),cl);
            }
        }
        wend = b;
    }
    hend = b;

}
