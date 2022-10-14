#include "readaline.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t readaline(FILE *inputfd, char **datapp) {
    if (inputfd == NULL || datapp == NULL) exit(EXIT_FAILURE);
    if (feof(inputfd)) {
        inputfd = NULL;
        return 0;
    }
    size_t max_size = 256;
    if ((*datapp = malloc(max_size * sizeof(char))) == NULL) exit(EXIT_FAILURE);
    //char buf[200];
    int ch = fgetc(inputfd);
    size_t ct = 0;
    while (ch != EOF) {
        (*datapp)[ct] = (char)ch;
        ct++;
        if (ct >= max_size) {
            max_size *= 2;
            *datapp = realloc(*datapp, max_size * sizeof(char));
        }
        if (ch == '\n') break;
        ch = fgetc(inputfd);
    }
    if (ct > 0) {
        if (ch != '\n' && ch != EOF) exit(4);
        else {      //use try
		// printf("Size: %ld\n", ct);
            *datapp = (char*) realloc(*datapp, (ct + 1) * sizeof(char));
            *(*datapp + ct) = '\0';
        }
    }
    return ct;
}
