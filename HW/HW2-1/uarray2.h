/*
* Authors:Andrew Crofts, Alex Tsuetaki
* Class: COMP40
* Homework2: iii
* filename: uarray2.h
* Date Modified: 9/27/19
* Purpose: header of the UArray2_T data structure
*
*/

#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "uarray.h"
#include "uarrayrep.h"
#include "mem.h"

#ifndef _UARRAY2_H_
#define _UARRAY2_H_




typedef struct UArray2_T {
        int width;
        int height;
        unsigned int size;
        UArray_T uarray;
} UArray2_T;


struct UArray2_T UArray2_new(int width_in, int height_in,unsigned int size_in);
void * UArray2_at(struct UArray2_T, int, int);
void UArray2_free(struct UArray2_T*);
void UArray2_map_col_major(UArray2_T, void apply(int height, int width, 
    UArray2_T a, void *value, void*cl), void *cl);
void UArray2_map_row_major(UArray2_T, void apply(int height, int width, 
    UArray2_T a, void *value, void *cl), void *cl);


int UArray2_width(struct UArray2_T);
int UArray2_height(struct UArray2_T);
int UArray2_size(struct UArray2_T);


#endif
