#include "readaline.h"
#include <stdlib.h>
#include <stdio.h>
#include <except.h>
#include <string.h>
#include <assert.h>

//readaline
//arguments:
//          inputfd: the file that is to be read from
//          datapp: a pointer to where the string will be returned to
//returns: a int representing the exit code (success or type of failure)
//purpose:
//        to read a line from a file then return that line to a variable in 
//        another program. Simlines for instance.
size_t readaline(FILE *inputfd, char **datapp) {
    if (inputfd == NULL || datapp == NULL) {
        fprintf(stderr, "ERROR: Both args to readaline are NULL\n");
        exit(EXIT_FAILURE);
    }
    if (feof(inputfd)) {
        inputfd = NULL;
        return 0;
    }
    size_t max_size = 256;
    // dynamically allocate memory
    assert((*datapp = malloc(max_size * sizeof(char))) != NULL);
    int ch = fgetc(inputfd);
    size_t ct = 0;
    while (ch != EOF) {
	    printf("%c",ch);
        (*datapp)[ct] = (char)ch;
        ct++;
        if (ct >= max_size) {
            max_size *= 2;
            *datapp = realloc(*datapp, max_size * sizeof(char));
        }
        if (ch == '.'|| ch == '?'|| ch =='!' ) break;
        ch = fgetc(inputfd);
    }
    if (ferror(inputfd)) {
        fprintf(stderr, "ERROR: Failed read the next character.\n");
        exit(EXIT_FAILURE);
    }
    if (ct > 0) {
	    //if (ch != '\n' && ch != EOF) exit(4);
	    //else {      //use try
            *datapp = (char*) realloc(*datapp, (ct + 1) * sizeof(char));
            (*datapp)[ct] = '\0';
	    //}
    }
    return ct;
}
