#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <stdbool.h>
#include <bit.h>
#ifndef BIT2_H_
#define BIT2_H_

typedef struct Bit2_T Bit2_T;
struct Bit2_T {
    int width;
    int height;
    Bit_T array;
};

Bit2_T Bit2_new(int, int);
int Bit2_get(Bit2_T, int, int);
int Bit2_put(Bit2_T, int , int , int);
void Bit2_free(Bit2_T*);
void Bit2_map_col_major(Bit2_T, void apply(int width, int height, 
                        Bit2_T a, int value, void *cl), void  *cl);
void Bit2_map_row_major(Bit2_T, void apply(int width, int height, 
                        Bit2_T a, int value, void *cl), void *cl);
int Bit2_width(Bit2_T);
int Bit2_height(Bit2_T);
#endif
