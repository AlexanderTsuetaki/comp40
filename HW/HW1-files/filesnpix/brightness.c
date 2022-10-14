#include <stdio.h>
#include <stdlib.h>
#include "pnmrdr.h"

FILE *get_input(int num_arg, char** arg_str);
float read_img(FILE*);

int main(int argc, char** argv)
{
    FILE* img = get_input(argc, argv);
    if (img == NULL) {
        fprintf(stderr, "Failed to open img file.\n");
        exit(EXIT_FAILURE);
    }
    printf("%1.3f\n", read_img(img));
    return 0;
}

FILE *get_input(int num_arg, char **arg_str) {
    if (num_arg == 1) return stdin;
    else if (num_arg == 2) return fopen(arg_str[1], "r");
    else return NULL;
}

float read_img(FILE* img) {
    Pnmrdr_T reader = Pnmrdr_new(img);
    int sum = 0, width = Pnmrdr_data(reader).width, 
        height = Pnmrdr_data(reader).height;
    for (int i = 0; i < width * height; i++) {
        sum += Pnmrdr_get(reader);
    }
    return (float)(sum/255.0/height/width);
}