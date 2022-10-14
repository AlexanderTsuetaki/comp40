/*unblackedges.c
 *Purpose: To remove all black edges of a image in PBM format
 *
 *Authors: Alexander Tsuetaki, Andrew Crofts
 *Date Modified: 9/27/19
 */
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <except.h>
#include "bit2.h"
#include <list.h>
#include <pnmrdr.h>

FILE *get_input(int num_arg, char** arg_str);
Bit2_T pbmread (FILE * input);
List_T add_initial_edges(Bit2_T bits, List_T  st);
List_T add_around(Bit2_T bits, List_T st, int width, int height);
List_T push_new_loc(int width, int height, List_T st);
List_T white_edges(Bit2_T bits, List_T st);
void pbmwrite(FILE * outputfp, Bit2_T bitmap);
void testprint(int width, int height, Bit2_T a , int value, void * cl);

typedef struct loc loc;
struct loc{
    int width;
    int height;
};

/*main:
 *purpose: to  run the function by
 *Arguments: int argc: the number of arguments on the command line
 *           int argv: a array of char pointers representing command line words
 *Returns: int: 0 if successful other numbers of not;
 */
int main(int argc, char** argv)
{
    Bit2_T picture;
    List_T stack = List_list(NULL);
    FILE *img = get_input(argc,argv);
    assert(img);
    if(img == NULL){
        fprintf(stderr,"ERROR: Failed to open ing file.\n");
        EXIT_FAILURE;
    }
    picture = pbmread(img);
    stack = add_initial_edges(picture,stack);
    stack = white_edges(picture,stack);
    List_free(&stack);
    pbmwrite(img, picture);
    fclose(img);
    return EXIT_SUCCESS;
}

/*get_input
 *purpose: to get a file from either stdin or a command line
 *Arguments:int num_arg: the number of command line arguments
 *         :char** argc_str:an array of strings represening command line words
 *Returns: FILE*: a File pointer
 */
FILE *get_input(int num_arg, char ** argc_str)
{
    if(num_arg==1){          
        return stdin;
    }
    else if (num_arg == 2){
        return fopen(argc_str[1], "r");
    }
    else{
        return NULL;
    }
}


/*pbmread:
 *purpose: to populate a 2d bit array with the values from a pbm file
 *Arguments:FILE * input: a file pointer, hopefully to a pbm file
 *Returns: Bit2_T: a 2d array of bits
 */
Bit2_T pbmread (FILE * input)
{
    assert(input != NULL);
    Pnmrdr_T reader;
    Bit2_T bits;
    int width;
    int height;
    TRY
        reader = Pnmrdr_new(input);
    EXCEPT(Pnmrdr_Badformat)
        Pnmrdr_free(&reader);
        fprintf(stderr,"ERROR:Input is not in the correct format.\n");
    END_TRY;
    width = Pnmrdr_data(reader).width;
    height = Pnmrdr_data(reader).height;
    if((widthemacs  * height)==0){
        fprintf(stderr,"ERROR: Pixel count is 0.\n");
        exit(EXIT_FAILURE);
    }
    bits = Bit2_new(width,height);
    for(int height  =  0 ; height < Bit2_height(bits); height++){
        for(int width = 0; width < Bit2_width(bits); width++){
                Bit2_put(bits, width , height , Pnmrdr_get(reader));
        }
    }   
    Pnmrdr_free(&reader);
    return bits;    
}
    
/*add_initial_edges
 *Purpose: to populate a stack with the initial black edges of a 2d bits array
 *Arguments: Bit2_T bits: a 2d array of bits to be queried
 *         : List_T st: a list object representing a stack of pixels to change
 *Returns: List_T: the new stack to update the stack that was passed in;
 */ 
List_T add_initial_edges(Bit2_T bits, List_T st)
{
    for(int width = 0; width < Bit2_width(bits); width++){
        if(Bit2_get(bits, width, 0) == 1){
            st =push_new_loc(width, 0,st);
        }
        if(Bit2_get(bits, width, Bit2_height(bits)-1) == 1){
            st =push_new_loc(width, Bit2_height(bits)-1,st);
        }
    }
    for(int height = 1; height < Bit2_height(bits); height++){
        if(Bit2_get(bits, 0, height) == 1){
            st =push_new_loc(0,height,st);
        }
        if(Bit2_get(bits, Bit2_width(bits)-1,height) == 1){
            st =push_new_loc(Bit2_width(bits)-1,height,st);
        }
    }
    return st;
}

/*push_new_loc
 *Purpose: to add a new location pointer to the stack;
 *Arguments: int width: the width part of the location to be added to the stack
 *        : int height:the height part of the location to be added to the stack
 *        : List_T st: a list representing the stack of pixels to change color
 *Returns: List_T : a List_T object to update the List_T being passed in
 */
List_T push_new_loc(int width, int height, List_T st){
    loc * point = calloc(1,sizeof(loc));
    (*point).width = width;
    (*point).height = height;
    st = List_push((st), point);
    return st;
}

/*white_edges
 *Purpose: To change all the edges locations in the stack to white
 *Arguemnts: Bit2_T bits: a 2d array of bits to be changed
 *         : List_T st: a stack of locations in the 2d array to be changed
 *Returns: List_T: a stack of locations that should be null;
 */
List_T white_edges(Bit2_T bits, List_T st)
{
    loc *  point ;
    void ** vpoint;
    while(st != NULL){
        vpoint = (void **)malloc(sizeof(void*));
        st = List_pop((st),vpoint);
        point = (*vpoint);
        if(Bit2_get(bits, (*point).width, (*point).height) == 1){
            Bit2_put(bits, (*point).width, (*point).height, 0);
            st =add_around(bits,st,(*point).width,(*point).height);
        }
        free(point);
        free(vpoint);
    }
    return st;
}

/*add_arround
 *Purpose:to add all black pixels locations around an edge pixel into the stack
 *Arguments: Bit2_T bits: a 2d array of bits to be queried
 *         : List_T st: a stack for locations
 *         : int width: the width location of the edge pixel being 
 *                      queried around
 *         : int height: the height location of the edge pixel being 
 *                      qyerued around
 *Returns: List_T: an updated stack with the new edge pixels added
 */
List_T add_around(Bit2_T bits, List_T st, int width, int height)
{
    loc * locations = (loc*)malloc(sizeof(loc*)*4);
    loc temp;
    temp.width = -1;
    temp.height = -1;
    for(int i = 0; i < 4; i++){
        locations[i] = temp;
    }
    temp.width = width;
    if(height > 0){
        temp.height = height-1;
        locations[0] = temp;
    }
    if(height < Bit2_height(bits)-1){
        temp.height = height+1;
        locations[1] = temp;
    }
    temp.height = height;
    if(width > 0){
        temp.width =  width-1;
        locations[2] = temp;
    }
    if(width < Bit2_width(bits)-1){
        temp.width = width+1;
        locations[3] = temp;
    }
    for(int i = 0; i < 4; i++){
       if(locations[i].width != -1){
           if(Bit2_get(bits, locations[i].width, locations[i].height)==1){
         st = push_new_loc(locations[i].width, locations[i].height,st);
         // printf("after push");
           }
       }
    }
    free(locations);
    return st;
}
    
/*pbmwrite
 *Purpose: to print out the new pbm file contenst to stdout 
 *Arguments: File * outputfp: the file being used to write to if needed
 *         : Bit2_T bitmap: a 2d array of bits to be printed;
 *Returns: none;
 */
void pbmwrite(FILE * outputfp, Bit2_T bitmap)
{
    (void)outputfp;
    int temp;
    printf("P1\n#we done it\n%d %d\n",Bit2_width(bitmap),Bit2_height(bitmap));
    Bit2_map_row_major(bitmap, testprint, &temp);
    Bit2_free(&bitmap);
}

/*testpring:
 *Purpose: prints out the bitmap to stdout;
 *Arguments: int width: the width component of the location of the bit 
 *                      to be printed
 *         : int height: the height component of the location of the bit
 *                       to be printed
 *         : Bit2_T a: a 2darray of bits to be queried and printed
 *         : int value: a value of at the height and width(UNUSED);
 *         : void * cl: a pointer to an int of value 0 (UNUSED);
 *Returns: none;
 *
 */
void testprint(int width, int height , Bit2_T a, int value, void * cl)
{
    (void) value;
    (void) cl;
    printf("%d", Bit2_get(a, width, height));
    if(width == Bit2_width(a)-1){
        printf("\n");
    }
}


