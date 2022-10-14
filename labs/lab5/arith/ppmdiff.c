/*ppmdiff
 *Purpose: to check if two images are the same amd [romts tp stdout
 *
 *Date Modified 10/4/19
 *Author Alexander Tsuetaki
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pnmrdr.h>
int main(int argc, char** argv())
{
        File I = fopen(argc_str[1],"r");
        File II = fopen(argc_str[2],"r");
        Pnmrdr_T reader1 = Pnmrdr_new(&I);
        Pnmrdr_T reader2 = Pnmrdr_new(&II);
        int I_width = Pnmrdr_data(reader1).width;
        int I_height = Pnmrdr_data(reader1).height;
        int II_width = Pnmrdr_data(reader2).width;
        int II_height = Pnmrdr_data(reader2).height;
	if(argc != 3){
		fprintf(stderr,"bad input");
		return 1;
	}
	
	if(I_width-II_height> 1 || I_width- IIwidth <-1){
		fprintf(stderr,"bad input");
		return 1;
	}
	if(I_height-II_height> 1 || I_height - II_height <-1){
		fprintf(stderr,"bad input");
		return 1;
	}

	if(I_width > II_width){
		width = I_width;
	}
	else{
		width = II_width;
	}
	if(I_height > II_height){
		height = I_height;}
	else{
		height = II_height;
	}
	printf("%1.3",get_number(width, height, reader1, reader2));
}

double get_number(

	int width;
	int height;
	int E = 0;
	for(int i = 0, i < width, i++){
		for(int j = 0; j < height, j++){
			E = E +(
		}
	}
	E= E/(3*width*height);
