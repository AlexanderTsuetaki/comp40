/*
  ppmtrans.c

  purpose: Implements different mapping methods to transform a picture

  authors: Gabriel McNinch, Alex Tsuetaki, and whoever wrote the starter code
  
  date: 10/09/19
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mem.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

typedef short bool;
#define true 1
#define false 0

typedef short mapping;
#define row 0
#define col 1
#define block 2

typedef A2Methods_UArray2 A2;

typedef struct Carepackage{
        A2Methods_T methods;
        A2 array;
} *Carepackage;


void mv_pixel(A2 arr, A2 arr2, A2Methods_T methods,
              int i,int j, int i2, int j2);
void apply_transpose(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_flip_horiz(int i, int j, A2 array, A2Methods_Object *ptr,void *cl);
void apply_flip_vert(int i, int j, A2 array, A2Methods_Object *ptr,void *cl);
void apply_rotate90(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_rotate180(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_rotate270(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_print(int i, int j, A2 array, A2Methods_Object *ptr, void *cl);
double run_flips(A2Methods_mapfun *map, A2 array, int rotation, Carepackage c,
                 bool *useArr2);
void print_timing(FILE * timings_file, int rotation, double timeused, 
                  mapping way_of_mapping);
A2 set_array2(A2Methods_T methods, A2 array, int rotation);

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

int main(int argc, char *argv[])
{
        FILE * picture = NULL;
    char *time_file_name = NULL;
    int   rotation       = 0;
    int   i;
    mapping way_of_mapping = row;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain;
    assert(methods);

    /* default to best map */
    A2Methods_mapfun *map = methods->map_default;
    assert(map);

    /* 
       Goes through each of the arguments and sets what methods to use
       and what transformations to do. And checks if there needs to be time
       file output
     */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-row-major") == 0) {
            SET_METHODS(uarray2_methods_plain, map_row_major,
                    "row-major");
            way_of_mapping = row;
        } else if (strcmp(argv[i], "-col-major") == 0) {
            SET_METHODS(uarray2_methods_plain, map_col_major,
                    "column-major");
            way_of_mapping = col;
        } else if (strcmp(argv[i], "-block-major") == 0) {
            SET_METHODS(uarray2_methods_blocked, map_block_major,
                "block-major");
            way_of_mapping = block;
        } else if (strcmp(argv[i], "-rotate") == 0) {
            if (!(i + 1 < argc)) {      /* no rotate value */
                usage(argv[0]);
            }
            char *endptr;
            rotation = strtol(argv[++i], &endptr, 10);
            if (!(rotation == 0 || rotation == 90 ||
                rotation == 180 || rotation == 270)) {
                fprintf(stderr, "Rotation must be 0, 90 180 or 270\n");
                usage(argv[0]);
            }
            if (!(*endptr == '\0')) {    /* Not a number */
                usage(argv[0]);
            }
        } else if (strcmp(argv[i], "-flip") == 0) {

            char *endptr = argv[++i];
            if (strcmp(endptr, "horizontal") == 0)
                rotation = 420;
            else if (strcmp(endptr, "vertical") == 0)
                rotation = 720;
            else {
                fprintf(stderr,
                    "Flipping can only be done horizontal or vertical\n");
            }
        } else if (strcmp(argv[i], "-transpose") == 0) {
            rotation = 69;

        } else if (strcmp(argv[i], "-time") == 0) {
            time_file_name = argv[++i];
        } else if (*argv[i] == '-') {
            fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
        } else if (argc - i > 1) {
            fprintf(stderr, "Too many arguments\n");
            usage(argv[0]);
        } else {
                if(picture == NULL){
                picture = fopen(argv[i],"r");
            }
            else {
                break;
            }
        }
    }
    /* If the picture file was not opened, take the image in via stdin */
        if(picture == NULL){
                picture = stdin;
        }
        assert(picture);
    /* Read in the picture file into a array */
        Pnm_ppm img = Pnm_ppmread(picture,methods);
        A2 array = img->pixels;
        A2 array2 = set_array2(methods, array, rotation);


        Carepackage c;
    NEW(c);
        c->array = array2;
        c->methods = methods;
    /* Create timings file and open it if its required */
    FILE *timings_file = NULL;
    if (time_file_name) {
        timings_file = fopen(time_file_name,"a");
        assert(timings_file);
    }
    bool useArr2 = true;

    double time_used = run_flips(map, array, rotation, c, &useArr2);
    
    /* Print to the timings file if its required */
    if (timings_file != NULL)
        print_timing(timings_file, rotation, time_used, way_of_mapping);
    
    /*
      If another array was created set the image to write to stdout
      to that new array and free the old array. If not free the empty unnused
      array
    */
    if(useArr2) {
        img->pixels = array2;
        img->width = methods->width(array2);
        img->height = methods->height(array2);
        methods->free(&array);
    } else {
        methods->free(&array2);
    }
    
    Pnm_ppmwrite(stdout,img);
    
    FREE(c);
    Pnm_ppmfree(&img);
    fclose(picture);
    exit(EXIT_SUCCESS);
}

/*
  set_array2
  
  purpose: Creates a new array with the correct dimensions based on what
  transformation is being done

  arguments: The methods to use to create the new array, the old array, and
  what type of transformation is being done

  returns: The new array
 */
A2 set_array2(A2Methods_T methods, A2 array, int rotation) {
    A2 array2;
    if(rotation == 90 || rotation == 270 || rotation == 69){
                array2 = methods->new(methods->height(array),
                         methods->width(array), methods->size(array));
        }
        else{
                array2 = methods->new(methods->width(array),
                         methods->height(array), methods->size(array));
        }
    return array2;
}

/*
  print_timing
  
  purpose: Prints the timing info to the given file using the transformation,
  how much time was used, and the method of mapping

  arguments: The given file, the transformation, the time used, and the
  method of mapping

  returns: None
 */
void print_timing(FILE * timings_file, int rotation, double timeused,
                mapping way_of_mapping){
    switch(way_of_mapping) {
        case row: fprintf(timings_file,"Row-Major-Mapping:\n");
            break;
        case col: fprintf(timings_file,"Col-Major-Mapping:\n");
            break;
        case block: fprintf(timings_file,"Block-Major-Mapping:\n");
            break;
    }
    fprintf(timings_file,"Case [%d]: took %.0f nanoseconds (%d seconds)\n",
    rotation, timeused, (int)(timeused / 1000000000));
    fclose(timings_file);

}

/*
  run_flips

  purpose: Performs the given transformation on the given array

  arguments: The mapping function to use on the array, the given array, the
  transformation, A struct with a new array to copy the transformed array into,
  and a boolean for whether the new array was actually used for copying

  returns: how much time it took to run the transformation
 */
double run_flips(A2Methods_mapfun *map, A2 array, int rotation, Carepackage c,
                bool *useArr2){
    CPUTime_T timer;
    timer = CPUTime_New();
    double time_used = 0;
    switch(rotation)
    {
        case 90: CPUTime_Start(timer);
            (*map)(array, apply_rotate90  , c);
            time_used = CPUTime_Stop(timer);
            break;
        case 180: CPUTime_Start(timer);
            (*map)(array, apply_rotate180  , c);
            time_used = CPUTime_Stop(timer);
            break;
        case 270: CPUTime_Start(timer);
            (*map)(array, apply_rotate270  , c);
            time_used = CPUTime_Stop(timer);
            break;
        case 0:
            *useArr2 = false;
            break;
        case 69: CPUTime_Start(timer);
            (*map)(array, apply_transpose  , c);
            time_used = CPUTime_Stop(timer);
            break;
        case 420: CPUTime_Start(timer);
            (*map)(array, apply_flip_horiz  , c);
            time_used = CPUTime_Stop(timer);
            break;
        case 720: CPUTime_Start(timer);
            (*map)(array, apply_flip_vert  , c);
            time_used = CPUTime_Stop(timer);
            break;
    }
    CPUTime_Free(&timer);
    return time_used;
}

/*
  apply_rotate90

  purpose: Rotates a single pixel of the array 90 degrees and copies it
  into a new array

  arguments: The indices of the pixel(i,j), the old array, the actual pixel,
  and a struct with the methods to use on arrays and the new array to copy to

  returns: None
 */
void apply_rotate90(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
        (void) ptr;
        Carepackage c = cl;
        mv_pixel(array, c->array, c->methods, i, j,
                 (c->methods)->height(array)-j-1, i);
}

/*
  apply_rotate180

  purpose: Rotates a single pixel of the array 180 degrees and copies it
  into a new array

  arguments: The indices of the pixel(i,j), the old array, the actual pixel,
  and a struct with the methods to use on arrays and the new array to copy to

  returns: None
 */
void apply_rotate180(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
        (void) ptr;
        Carepackage c = cl;
        mv_pixel(array, c->array, c->methods, i, j,
                 (c->methods)->width(array)-i-1,
                 (c->methods)->height(array)-j-1);
}

/*
  apply_rotate270

  purpose: Rotates a single pixel of the array 270 degrees and copies it
  into a new array

  arguments: The indices of the pixel(i,j), the old array, the actual pixel,
  and a struct with the methods to use on arrays and the new array to copy to

  returns: None
 */
void apply_rotate270(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
        (void) ptr;
        Carepackage c = cl;
        mv_pixel(array, c->array, (c->methods), i, j, j, 
                 (c->methods)->width(array) - i - 1);
}

/*
  apply_flip_horiz

  purpose: Flips a single pixel of the array horizontally and copies it
  into a new array

  arguments: The indices of the pixel(i,j), the old array, the actual pixel,
  and a struct with the methods to use on arrays and the new array to copy to

  returns: None
 */
void apply_flip_horiz(int i, int j, A2 array, A2Methods_Object *ptr,void *cl)
{
        (void) ptr;
        Carepackage c = cl;
    mv_pixel(array, c->array, (c->methods), i, j,
        (c->methods)->width(array)-i-1, j);
}

/*
  apply_flip_vert

  purpose: Flips a single pixel of the array vertically and copies it
  into a new array

  arguments: The indices of the pixel(i,j), the old array, the actual pixel,
  and a struct with the methods to use on arrays and the new array to copy to

  returns: None
 */
void apply_flip_vert(int i, int j, A2 array, A2Methods_Object *ptr, void *cl)
{
        (void) ptr;
        Carepackage c = cl;
    mv_pixel(array, c->array, (c->methods), i, j, i,
                 (c->methods)->height(array)-j-1);
}

/*
  apply_print

  purpose: Prints a single pixel of the array

  arguments: The indices of the pixel(i,j), the array, the actual pixel,
  and a struct with the methods to use on arrays

  returns: None
 */
void apply_print(int i, int j, A2 array, A2Methods_Object *ptr, void *cl)
{
    (void)j;
    Carepackage c = cl;
    Pnm_rgb pix = ptr;
    printf("%u %u %u",pix->red,pix->green,pix->blue);
    if(i == (c->methods)->width(array)-1)
        printf("\n");
    else
        printf("  ");
}

/*
  apply_transpose

  purpose: Transposes a single pixel of the array and copies it
  into a new array

  arguments: The indices of the pixel(i,j), the old array, the actual pixel,
  and a struct with the methods to use on arrays and the new array to copy to

  returns: None
 */
void apply_transpose(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
        (void) ptr;
        Carepackage c = cl;
        mv_pixel(array, c->array, (c->methods), i, j, j, i);
}

/*
  mv_pixel

  purpose: Moves a single pixel from arr1 at (i,j) to another position (i2,j2),
  in arr2

  arguments: The 2 arrays, the methods to use on arrays, the two coordinates
  in each of the arrays

  returns: None
 */
void mv_pixel(A2 arr, A2 arr2, A2Methods_T methods,
                int i,int j, int i2, int j2)
{
        Pnm_rgb pixel1 = methods->at(arr,i,j);
        Pnm_rgb pixel2 = methods->at(arr2,i2,j2);
    pixel2->red = pixel1->red;
    pixel2->green = pixel1->green;
    pixel2->blue = pixel1->blue;

}
