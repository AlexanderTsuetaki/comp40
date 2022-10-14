//Simlines.c
//Purpose: To find the "match pairs"(see below) in the the files named in the 
//         command line (not passed in)
//Authors: Tianyi Ma, Alexander Tsueaki
//Date Modified: 9/14/19


#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "table.h"
#include "list.h"
#include "atom.h"
#include "string.h"

typedef enum { false, true } bool;

void populate_table(int, char**, Table_T);
void print_match(Table_T);
void print_formated(char*,int);
bool parse_str(char**, int);
bool is_char(char);
static void vfree(const void*, void**, void*);

struct Location {
    char* filename;
    int lineno;
};

//main
//parameters :
//            int argc:number of command line arguments
//            char** argv: array of command line arguments srings
//purpose: to drive the simline function by creating and populating a table
//         then printing all the match pairs found in the files names in argv
//         then destructs the table
int main(int argc, char** argv) {
    Table_T t = Table_new(8192, NULL, NULL);
    populate_table(argc, argv, t);
    print_match(t);
    Table_map(t, vfree, NULL);
    Table_free(&t);
    return EXIT_SUCCESS;
}

//Populate_table
//parameters : 
//            argc:number of command line arguments
//            argv:array of strings representing names of files
//purpose: to populate a hanson hash table with the data provided from the
//         files represented in argv

void populate_table(int argc, char** argv, Table_T t) {
    FILE *curr;
    char *line;
    List_T list;
    int line_ct;
    //checking if there are files to be parsed
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
	    //temporary string to hold the lines of a file
            curr = fopen(argv[i], "r");
	    // TODO: create try-except statement for error
            if (curr == NULL) exit(EXIT_FAILURE);
	    line_ct = 1;
	    int size = readaline(curr, &line);
            while (size > 0) {
		const char * temp;
                if (parse_str(&line,size) == true) {
		    //formating data from a file into structures then an atom 

			//TODO: NOT WORKING 
                    struct Location *loc = malloc(sizeof(struct Location));
                    struct Location this_loc = {argv[i], line_ct};
                    *loc = this_loc;
		    temp = line;
                    const char* temp_a = Atom_string(temp);
		    //checking if the sentence exists already
		    // if it exists it adds it to the list in the structure
		    // else it enters the new struct representing the line in
                    list = Table_get(t, temp_a);
                    if (list == NULL) {
                        Table_put(t, temp_a, List_list(loc, NULL)); 
                    }
                    else Table_put(t, temp_a, List_push(list, loc));
                }
                free(line);
                line_ct++;
            }
            fclose(curr);
        }
    }
}

//parse_str
//arguments:
//           s: a pointer to a string to be parsed
//           size: size of line being passed in
//purpose: to parse a string into a uniform format such that words are 
//         separated by one space (ends on a newline/eof) 
//         
bool parse_str(char** s, int size) {
    char buf[size];
    int c_ct = 0;
    bool is_word = false;
    for (int i = 0; (*s)[i] != '\n'; i++) {
        char c = (*s)[i];
        if (is_char(c) == true) {
            if (is_word == false) {
                is_word = true;
            }
            buf[c_ct] = c;
            c_ct++;
        }
        else if (is_word == true) {
            is_word = false;
            buf[c_ct] = ' ';
            c_ct++;
        }
    }
    if (c_ct > 1) {
        memmove(*s, buf, c_ct);
        if (is_word == false) (*s)[c_ct - 1] = '\0';
        else (*s)[c_ct] = '\0';
        return true;
    }
    else return false;
}


//is_char
//arguments:
//          c: a char to be validated
//purpose: to determine of a character is a word character or not
//        
bool is_char(char c) {
    //printf("%c", c);
    if ((c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z')) 
        return true;
    else return false;
}

//print_match
//arguments:
//          t: the main hanson table representing all sentences in files
//prupose: to print a match pair in the format given in the 2019 comp40-lab1
//         design specification
void print_match(Table_T t) {
    List_T list;
    bool first = true;
    void **array = Table_toArray(t, NULL);
    int len = Table_length(t);
    for (int i = 0; i < len * 2; i+= 2) {
        list = array[i+1];
        list = List_reverse(list);
        void *temp;
        struct Location* loc;
	//prints only if there is an actural match pair (fixed)
	if(List_length(list) > 1){
		if(!first){ //added since previous version was not corectally
			printf("\n");//adding the newline (cant tell last
		}                    //instance group while running);
		first = false;
		printf("%s\n", (char*)array[i]);
		//printing all words from a the list in the array;
		while (list != NULL) {
			list = List_pop(list, &temp);
			loc = temp;
			print_formated((*loc).filename, (*loc).lineno);
			free(loc);
		}
	}
	else{
		list = List_pop(list, &temp);
		loc = temp;
		free(loc);
	}
    }
    free(array);
}

//print_formated
//arguments:
//          filename: a string representing the name of the file a line is in
//          linenum: a int representing the line number of a given line
void print_formated(char * filename, int linenum){
        int digits = 0;
	int tempnum = linenum;
	if(tempnum == 0)
		tempnum++;
	while(tempnum > 0){
		tempnum = tempnum/10;
		digits++;
	}
	int filename_length = 20-strlen(filename);//buffer for file name
	int number_length = 7-digits;//buffer for line number
	printf("%s", filename);
	for(int i = 0; i < filename_length; i++){
		printf("-");
	}
	printf("-");
	for(int j = 0; j < number_length; j++){
		printf("-");
	}
	printf("%d\n", linenum);
}

//vfree
//arguments:
//          key: key of to an address in the hash table
//          value: value in an address in a hash table
//          cl: TODO:
static void vfree(const void *key, void **value, void *cl) {
    (void) key;
    (void) cl;
    (void) value;
    //FREE(*value);
}
