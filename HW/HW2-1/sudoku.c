/*
* Authors:Andrew Crofts, Alex Tsuetaki
* Class: COMP40
* Homework2: iii
* filename: Sudoku.c
* Date Modified: 9/27/19
* Purpose: implementation of sudoku checker, checks if sudoku board is 
*          correct 
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include <assert.h>
#include "uarray2.h"
#include "uarray.h"

FILE *open_file(int argc, char **filename);
void play_sudoku(struct UArray2_T);
UArray2_T populate_sudoku_array (FILE *img);
void find_correct(int height, int width, UArray2_T , void *value, void*cl);
bool array_all_true(bool *bool_array);
void reset_bool_array(bool *bool_array_in); 
void check_box(UArray2_T array_in, bool *bool_array_in);
void is_box_correct(UArray2_T array_in, int width_value, int height_value, 
  bool* bool_array_in);


const int SIZE_OF_BOOL_ARRAY = 9; 


int main (int argc, char *argv[])
{
  FILE *grayscale = open_file(argc, argv); 
  assert(grayscale);
  UArray2_T sudoku_array = populate_sudoku_array(grayscale);
  play_sudoku(sudoku_array);
  fclose(grayscale);       
  UArray2_free(&sudoku_array);

    return 0;
}


/*
* Name: open_file
* Purpose: opens the file from either the given filename or standard in
* Arguments: the number of command line argurments and the filename
* returns: file pointer to opened file, unless a problem is encountered,
* after
*/
FILE *open_file(int argc, char **filename) 
{
    if (argc == 1) { 
        return stdin; 
    } 
    else if (argc == 2) {
        return fopen(filename[1], "r");       
    }
    else{
        return NULL; 
    }
}


/*
* Name: populate_sudoku_array
* Purpose: creates a 2D array (UArray2_T) and populates it with values from 
*          the pgm file  
* Arguments: file pointer to opened pgm file
* returns: the completely populated 2D sudoku array  
*/
UArray2_T populate_sudoku_array (FILE *img)
{
     Pnmrdr_T reader;

    TRY
          reader = Pnmrdr_new(img); 
    EXCEPT(Pnmrdr_Badformat)   
          fprintf((stderr), "ERROR: INPUT IS NOT IN THE CORRECT FORMAT\n");
          exit(1); 
    END_TRY;

    int arr_width = Pnmrdr_data(reader).width;  
    int arr_height = Pnmrdr_data(reader).height;  
    UArray2_T Sudoku_U2Array = UArray2_new(arr_width, arr_height, sizeof(int));
    int *vptr; 
    int temp; 
  for (int i = 0; i < arr_width; i++){
      for (int j = 0; j < arr_height; j++){
          temp = (int)Pnmrdr_get(reader);
          if (((temp > 9) || (temp < 1)) || (arr_width != SIZE_OF_BOOL_ARRAY)
            || (arr_height != SIZE_OF_BOOL_ARRAY)){
            fclose(img);
            UArray2_free(&Sudoku_U2Array); 
            fprintf(stderr,"ERROR: FILE CONTAINS INCORRECT SUDOKU FORMATTING");
            fprintf(stderr, "\n");
            exit(1);
          }
        vptr = UArray2_at((Sudoku_U2Array), i, j);
        *vptr = temp;   
      }
     }

     Pnmrdr_free(&reader); 
     return Sudoku_U2Array; 
}    


/*
* Name: play_sudoku
* Purpose: runs functionality of sudoku program by checking through each set
*         of 9 if there are repeats of any numbers
* Arguments: UArray2_T 2D array representing the sudoku board
* returns: none
*/
void play_sudoku(UArray2_T array_in)
{
  
bool bool_array[9] = {false, false, false,
                                       false, false, false,
                                       false, false, false};  

  UArray2_map_col_major(array_in, find_correct, bool_array);  
  UArray2_map_row_major(array_in, find_correct, bool_array);
  check_box(array_in, bool_array);

}



/*
* Name: find_correct
* Purpose: acts as the apply function in the 2D map functions, checks if the 
*          supplied value (in the form of an int) has already occurred in the
*          row/col/box
* Arguments: integer representing height index of value, integer representing 
*            width index of value, UArray2_T: 2D array being mapped 
*            through - the sudoku board, void pointer pointing to value at 
*            which the apply is being called, void pointer client pointer 
*            for use of any other value - in this case used to pass the
*            bool array
* returns: none 
*/
void find_correct(int height, int width, UArray2_T array_in, void *value, 
  void *cl)
{ 

  bool *bool_array = cl;  
 int index = (*(int*)value)-1;
  
  if (!bool_array[index]){
    bool_array[index] = true;
  }
  else{
    UArray2_free(&array_in);
    exit(1);
  }

 if (array_all_true(bool_array)){
    reset_bool_array(bool_array); 
  }

  (void)height; 
  (void)width; 
  (void)array_in; 

}



/*
* Name: reset_bool_array
* Purpose: converts all values in bool array back to false once there is a new
*          row/col/box
* Arguments: boolean array of completed line 
* returns: none  
*/
void reset_bool_array(bool  *bool_array_in)
{
  for (int i = 0; i < SIZE_OF_BOOL_ARRAY; i++){
    bool_array_in[i] = false; 
  }
}


/*
* Name: check box
* Purpose: acts similarly to map functions, moves through each subsquare
*          in the 2D array and checks to see if any numbers are repeated
* Arguments: 2D Array of sudoku board, bool array of all false to start
*            the process 
* returns: none
*/
void check_box(UArray2_T array_in, bool *bool_array_in)
{ 
  reset_bool_array(bool_array_in);

  for (int j = 1; j <= 7; (j = j + 3)){
    for (int k = 1; k <= 7; (k = k+3)){
      is_box_correct(array_in, j,k, bool_array_in); 
    }
  }
}


/*
* Name: array_all_true
* Purpose: checks if boolean array is composed of only true values
* Arguments: boolean array 
* returns: boolean representing whether the boolean array is all true
*          values or not(ftrue if all true, alse if not)
*/
bool array_all_true(bool *bool_array)
{
  for (int i = 0; i < SIZE_OF_BOOL_ARRAY; i++){
    if (!bool_array[i]){
      return false; 
    }
  }

  return true; 
}



/*
* Name: is_box_correct
* Purpose: helper funciton of check_box, checks box with center at given point
* Arguments: 2D array of sudoku board, integer representing width index,
*            integer representing height index in array, array of bools for 
*            corresponding values in box
* returns: none
*/
void is_box_correct(UArray2_T array_in, int width_value, int height_value,
  bool *bool_array)
{
    void *taken_value; 
      for (int i = -1; i < 2; i++){
        for (int k = -1; k < 2; k++){
            taken_value = UArray2_at(array_in, (height_value + i), 
              (width_value + k)); 
            find_correct((height_value + i), (width_value + k), array_in,
            taken_value, (void *)bool_array); 
        }
      }
}


