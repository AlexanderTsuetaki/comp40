//brightness.c
//Purpose: Implementation of brightness; read the grayscale value of each
//         pixel in the input file, and compute the average brightness
//         from a scale of 0 to 1.
//Authors: Tianyi Ma, Alexander Tsueaki
//Date Modified: 9/14/19
#include "brightness.h"

//main 
//arguments:
//          argc: an int representing the number of command line arguments
//          argv: an array of char arrays representing an array of the strings 
//                representing the names of files being passed to brightness
//returns: a int representing the exit code (success or type of failure)
//purpose: driver of the function finds if there is a picture to be parsed
//         then prints the brightness found by Pnmrdr
//
int main(int argc, char** argv)
{
    FILE* img = get_input(argc, argv);
    if (img == NULL) print_err("ERROR: Failed to open img file.\n");
    printf("%1.3f\n", read_img(img));
    return EXIT_SUCCESS;
}

//get_input
//arguments:
//           num_arg: an int representing the number of arguments given in the
//           command line to sets the input accordingly
//            arg_str: the arguments passed in in argv in main
//returns: a file to be parsed in read_img(see below)
//purpose: to determine the correct input method depending on if there is one 
//         file passed in or if there are no files passed in. returns null if 
//         more than two arguments are in the command line
FILE *get_input(int num_arg, char **arg_str) {
    if (num_arg == 1) return stdin;
    else if (num_arg == 2) return fopen(arg_str[1], "r");
    else return NULL;
}

//read_img
//arguments:
//          img: a file pointer that points to a file that contains a picture
//          to be parsed
//returns: float representing average brightness
//purpose: to parse a file's picture then return the average brightness of it.
//
float read_img(FILE* img) {
    Pnmrdr_T reader;
    TRY
        reader = Pnmrdr_new(img);
    EXCEPT(Pnmrdr_Badformat);
        print_err("ERROR: Input is not in the correct format.\n");
    END_TRY;
    int width = Pnmrdr_data(reader).width, 
        height = Pnmrdr_data(reader).height,
        sum = 0;
    if ((width * height) == 0) 
        print_err("ERROR: Pixel count is 0.\n");
    for (int i = 0; i < width * height; i++) {
        sum += Pnmrdr_get(reader);
    }
    return (float)(sum/255.0/height/width);
}

//print_err
//arguments:
//          err_msg: the error message to print in stderr
//returns: none
//purpose: to report an error and terminate the program
//
void print_err(char* err_msg) {
    fprintf(stderr, err_msg);
    exit(EXIT_FAILURE);
}