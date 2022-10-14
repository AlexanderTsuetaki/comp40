/*bit2.c
*purpose: holds the Bit2_T structure
*Authors: Alexander Tsuetaki, Andrew Crofts
*Date Modified: 9/27/19
*/

#include <stdio.h>
#include <bit.h>
#include "bit2.h"
#include <stdlib.h>
#include <assert.h>

/*Bit2_new:
*Purpose: creates new Bit2_T object;
*Arguments: int width: the width of the bit-array to be created
*           int height: the height of the bit-array to be created
*returns:Bit2_T new_bit_arr: the new Bit2_T object
*/
Bit2_T Bit2_new(int width, int height)
{   
    assert(width>0 && height > 0);
    Bit2_T new_bit_arr;
    new_bit_arr.width = width;
    new_bit_arr.height = height;
    new_bit_arr.array = Bit_new(width*height);
    return new_bit_arr;
}

/*Bit2_get:
*Purpose: returns the value at the given width and height 
*Arguments: Bit2_T arr: the array to be searched
*         : int width: the width value to be searched in
*         : int height: the height value to be searched in
*returns: int: 1 or 0, whatever the bit at [width, height] is set to;
*/
int Bit2_get(Bit2_T arr, int width, int height)
{
    return Bit_get(arr.array,height*arr.width + width);
}

/*Bit2_put:
*Purpose: to assign a value to a given point in a 2d bit array
*Arguments: Bit2_T arr: the array to be searched
*         : int width: the width value of the point to be changed
*         : int height: the height value of the point to be changed
*         : int value: the value to be added to the point to be changed
* Returns: int: the int previously at location [width, height]
*/
int Bit2_put(Bit2_T arr,int width, int height, int value)
{
    return Bit_put(arr.array, height*arr.width+width, value);
}

/*Bit2_free:
*Purpose: to free all of the bits assigned in the Bit2_T object;
*Arguments: Bit2_T * arrp: a pointer to a Bit2_T object to be deleted'
*Retruns: None
*/
void Bit2_free(Bit2_T *arrp)
{
    Bit_free(&(*arrp).array);
}
/*Bit2_map_row_major:
*Purpose: to map over the entire 2D bit array and apply a function
*       : maps a row then goes to the next row.
*Arguments: Bit2_T arr: the array to be mapped over
*         : void apply: a function to be applied to all values in the array
*         : void *cl : a void pointer to be used to check on items in apply
*                  : passed into apply 
*Returns: None
*/
void Bit2_map_row_major(Bit2_T arr, void apply(int width, int height, 
Bit2_T a,    int value, void *cl), void *cl)
{
    for(int i = 0; i < arr.height; i++){
        for(int j = 0; j < arr.width; j++){
            apply(j, i, arr, Bit2_get(arr,j,i),cl);
        }
    }
}

/*Bit2_map_row_major:
*Purpose: to map over the entire 2D bit array and apply a function
*       : maps a column then goes to the next column.
*Arguments: Bit2_T arr: the array to be mapped over
*         : void apply: a function to be applied to all values in the array
*         : void *cl : a void pointer to be used to check on items in apply
*                  : passed into apply 
*Returns: None
*/            
void Bit2_map_col_major(Bit2_T arr, void apply(int width, int height, 
Bit2_T a, int value, void *cl), void *cl)
{
    for(int i= 0; i < arr.width; i++){
        for(int j = 0; j < arr.height; j++){
            apply(i, j, arr, Bit2_get(arr,i,j),cl);
        }
    }
}

/*Bit2_width:
*Arguments: Bit2_T arr: a 2D bit array whos width value must be returned
*Returns: int: the width of the 2D bit array;
*/
int Bit2_width(Bit2_T arr)
{
    return arr.width;
}

/*Bit2_height:
*Arguments: Bit2_T arr: a 2D bit array whos height value must be returned
*Returns: int: the height of the 2D bit array;
*/
int Bit2_height(Bit2_T arr)
{
   return arr.height;
}
