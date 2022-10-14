//simlines.h
//Purpose: Interface for simlines
//Authors: Tianyi Ma, Alexander Tsueaki
//Date Modified: 9/14/19

#include "parseline.h"
#include "readaline.h"
#include "table.h"
#include "list.h"
#include "atom.h"
#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef SIMLINES_H_
#define SIMLINES_H_

void populate_table(int, char**, Table_T);
void print_match(Table_T);
void print_formated(char*,int);

struct Location {
    char* filename;
    int lineno;
};

#endif
