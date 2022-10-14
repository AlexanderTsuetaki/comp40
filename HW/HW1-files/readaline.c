
#include "readaline.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//readaline
//arguments:
//          inputfd: the file that is to be read from
//          datapp: a pointer to where the string will be returned to
//purpose:
//        to read a line from a file then return that line to a variable in 
//        another program. Simlines for instance.
size_t readaline(FILE *inputfd, char **datapp) {
    int size = 256;
    if (inputfd == NULL || datapp == NULL) exit(EXIT_FAILURE);
    if (feof(inputfd)) {
        inputfd = NULL;
        return 0;
    }
    char *buf = (char*)malloc(size * sizeof(char));
    int ch = fgetc(inputfd);
    size_t ct = 0;
    char *temp;
    while ((ch != EOF && ch != '\n') && ct < (size_t)size) {
	    //yprintf("%c",ch);
	buf[ct] = (char)ch;
        ct++;
        ch = fgetc(inputfd);
	if(ct == (size_t)size-1){
		size = size*2;
		temp = (char*)calloc(size, sizeof(char));
		for(int i = 0; i <=(int)ct;i++){
			temp[i] = buf[i];
		}
		free(buf);
		buf = temp;
		//free(temp);
		//printf("\nreallocated to size of %d\n%s\n",size,buf);
		buf[ct] = ch;
	}
    }
    if (ct > 0) {
        if (ch != '\n' && ch != EOF) exit(4);
        else { 
	    *datapp = (char*)calloc(ct,sizeof(char));
	    //printf("\ndebug:%s\n",buf);
            memmove(*datapp, buf, sizeof(char) * (ct));
	    //free(temp);
	}
    }
    //free(temp);
    free(buf);
    return ct;
}

// size_t readaline(FILE *inputfd, char **datapp) {
//     if (inputfd == NULL || datapp == NULL) exit(EXIT_FAILURE);
//     if (feof(inputfd)) {
//         inputfd = NULL;
//         return 0;
//     }
//     int buf_size = 256;
//     char *buf = malloc(sizeof(char) * buf_size);
//     int ch = fgetc(inputfd);
//     int ct = 0;
//     while (ch != EOF) {
//         //printf("%d\n", ct);
//         buf[ct] = (char)ch;
//         ct++;
//         if (ct == buf_size) {
//             buf_size *= 2;
//             buf = realloc(buf, buf_size);
//         }
//         if (ch == '\n') break;
//         ch = fgetc(inputfd);
//     }
//     if (ct > 0) {
//         if (ch != '\n' && ch != EOF) exit(4);
//         else {      //use try
//             //*datapp = malloc(sizeof(char) * (ct + 1));
//             // *datapp = malloc(sizeof(char) * ct);
//             // memmove(*datapp, buf, sizeof(char) * ct);
//             //(*datapp)[ct] = '\0';
//             //free(*datapp);
//             buf = realloc(buf, sizeof(char) * ct);
//             *datapp = malloc(sizeof(char) * ct);
//             memmove(*datapp, buf, ct * sizeof(char));
//             free(buf);
//         }
//     }
//     //printf("%ld\n", ct);
//     printf("DONE");
//     return ct;
// }
