#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

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


typedef A2Methods_UArray2 A2;

typedef struct Carepackage{
	A2Methods_T methods;
	A2 array;
}Carepackage;


void mv_pixel(A2 arr, A2 arr2, A2Methods_T methods, 
	      int i,int j, int i2, int j2);
void apply_transpose(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_flip(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_rotate90(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_rotate180(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);
void apply_rotate270(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl);




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

        /* default to UArray2 methods */
	A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
				    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
				    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
					"Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
				argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
			if(picture == NULL){
				picture = fopen(argv[i],"r");; 
			}
			else{
				break;
			}
                }
        }
	if(picture == NULL){
		picture = stdin;
	}
	assert(picture);
	Pnm_ppm img = Pnm_ppmread(picture,methods);
	A2 array = img->pixels;
	A2 array2;
	if(rotation == 90 || rotation == 270){
		array2 = methods->new(methods->height(array),
                         methods->width(array), methods->size(array));
	}
	else{
		array2 = methods->new(methods->width(array),
                         methods->height(array), methods->size(array));
	}
	Carepackage c;
	c.array = array2;
	c.methods = methods;
	(void) time_file_name;
	(*map)(array, apply_rotate90  , &c);

        assert(0);              // the rest of this function is not yet implemented
}


void apply_rotate90(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
	(void) ptr;
	Carepackage *c = cl;
	mv_pixel(array, c->array, c->methods, i, j, 
		 (c->methods)->height(array)-j-1, i);
}
 
void apply_rotate180(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
	(void) ptr;
	Carepackage *c = cl;
	mv_pixel(array, c->array, c->methods, i, j,
		 (c->methods)->width(array)-i-1, 
                 (c->methods)->height(array)-j-1);   
}

void apply_rotate270(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
	(void) ptr;
	Carepackage *c = cl;
	mv_pixel(array, c->array, (c->methods), i, j, j, 
		 (c->methods)->width(array)-i-1);
} 

void apply_flip_horiz(int i, int j, A2 array, A2Methods_Object *ptr,void *cl)
{
        (void) ptr;
	Carepackage *c = cl;
 	mv_pixel(array, c->array, (c->methods), i, j, i, 
		 (c->methods)->width(array)-j-1);
} 

void apply_flip_vert(int i, int j, A2 array, A2Methods_Object *ptr, void *cl)
{
	(void) ptr;
	Carepackage *c = cl;
        mv_pixel(array, c->array, (c->methods), i, j, 
		 (c->methods)->height(array)-i-1, j);
}




void apply_transpose(int i, int j, A2 array, A2Methods_Object *ptr,  void *cl)
{
	(void) ptr;
	Carepackage *c = cl;
	mv_pixel(array, c->array, (c->methods), i, j, 
		 (c->methods)->height(array)-j-1,
		 (c->methods)->width(array)-i-1);
} 

void mv_pixel(A2 arr, A2 arr2, A2Methods_T methods, 
                int i,int j, int i2, int j2)
{
	Pnm_rgb *pixel1 = methods->at(arr,i,j);
	Pnm_rgb *pixel2 = methods->at(arr2,i2,j2);
        *pixel2 = *pixel1; 
}
