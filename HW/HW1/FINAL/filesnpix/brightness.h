//brightness.h
//Purpose: Interface for brightness
//Authors: Tianyi Ma, Alexander Tsueaki
//Date Modified: 9/14/19

#include <stdio.h>
#include <stdlib.h>
#include <except.h>
#include "pnmrdr.h"

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

// Function prototypes
FILE *get_input(int num_arg, char** arg_str);
float read_img(FILE*);
void print_err(char*);

#endif