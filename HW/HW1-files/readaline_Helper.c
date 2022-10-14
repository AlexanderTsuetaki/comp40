//readaline_Helper.c
//Runs the readaline file for debugging purposes
//Alexander Tsuetaki, Tainyi Ma
//Comp40 - 2019

#include "readaline.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

int main(int argc, char* argv[]){	
	(void) argv;
	(void) argc;
	FILE *f = fopen("test1.txt", "r");
	char *datapp;
	int size = readaline(f, &datapp);
	while(size > 0){
		for(int i = 0; i< size; i++){
			printf("%c",datapp[i]);
		}
		printf("\n");
		size = readaline(f, &datapp);
	}
	fclose(f);
}


