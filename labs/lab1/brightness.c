#include <stdio.h>
#include <stdlib.h>
#include "pnmrdr.h"

int main(int argc, char **argv){

	char buffer[500]; 

	printf("%d", argc);

	FILE *fptr;
		
	if(fptr == NULL){
		printf("Error! \n");
		exit(0);
	}
	Pnmrdr_T img = Pnmrdr_new(scanf("d",&val));
	printf ("%d", Pnmrdr_get(img));

	
       
	fclose(fptr);

	printf("Hello, world!\n");
}

