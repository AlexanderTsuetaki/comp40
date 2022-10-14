//simlines.c
//Purpose: To find the "match pairs"(see below) in the the files named in the 
//         command line (not passed in)
//Authors: Tianyi Ma, Alexander Tsueaki
//Date Modified: 9/14/19
#include "simlines.h"

//main
//parameters :
//            int argc:number of command line arguments
//            char** argv: array of command line arguments srings
//returns: a int representing the exit code (success or type of failure)
//purpose: to drive the simline function by creating and populating a table
//         then printing all the match pairs found in the files names in argv
//         then destructs the table
int main(int argc, char** argv) {
    Table_T t = Table_new(8192, NULL, NULL);
    populate_table(argc, argv, t);
    if (Table_length(t) > 0) print_match(t);
    Table_free(&t);
    return EXIT_SUCCESS;
}

//Populate_table
//parameters : 
//            argc:number of command line arguments
//            argv:array of strings representing names of files
//returns: NONE
//purpose: to populate a hanson hash table with the data provided from the
//         files represented in argv
void populate_table(int argc, char** argv, Table_T t) {
    FILE *curr;
    char *line;
    List_T list;
    size_t char_ct, line_ct;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            curr = fopen(argv[i], "r");
            assert(curr != NULL);
            const char* temp_a;
            line_ct = 1;
            while ((char_ct = readaline(curr, &line)) > 0) {
		    printf("here");
                if (parseline(&line, char_ct) == true) {
                    struct Location *loc = malloc(sizeof(struct Location));
                    assert(loc != NULL);
                    struct Location this_loc = {argv[i], line_ct};
                    *loc = this_loc;
                    temp_a = Atom_string(line);
                    list = Table_get(t, temp_a);
                    if (list == NULL) {
                        Table_put(t, temp_a, List_list(loc, NULL));
                    }
                    else Table_put(t, temp_a, List_push(list, loc));
                }
                free(line);
                line_ct++;
            }
            if (line != NULL && line_ct == 1) free(line);
            fclose(curr);
        }
    }
}

//print_formated
//arguments:
//          filename: a string representing the name of the file a line is in
//          linenum: a int representing the line number of a given line
//returns:NONE
//purpose:to print all the match groups found in the files passed in and free
//        all memory in the hash table(including in the linked lists in the 
//        hash table)
void print_match(Table_T t) {
    List_T list;
    bool first = true;
    void **array = Table_toArray(t, NULL);
    int len = Table_length(t);
    //printf("%d",len);
    for (int i = 0; i < len * 2; i+= 2) {
        list = array[i+1];
        list = List_reverse(list);
        void *temp;
        struct Location* loc;
        //prints only if there is an actural match pair (fixed)  
        if(List_length(list) > 1){
         if(!first){ //added since previous version was not corectally
             printf("\n");//adding the newline (cant tell last
         }//instance group while running); 
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
        List_free(&list);
    }
    free(array);
}


//print_formated
//arguments:
//          filename: a string representing the name of the file a line is in
//          linenum: a int representing the line number of a given line  
//returns:none;
//purpose: to formate the print out statement in such that there is a 20 space
//         buffer for letter, a 1 space spacer, and a 7 digit buffer for 
//         line numbers.
void print_formated(char * filename, int linenum){
        int digits = 1;
        int tempnum = linenum;
        for(;tempnum /= 10, tempnum > 0; digits++);
        int filename_len = 20 - strlen(filename);   //buffer for file name
        int number_len = 7 - digits;                //buffer for line number 
        printf("%s", filename);
	printf(" ");
        for(int i = 0; i < filename_len + 1 + number_len; i++) printf(" ");
        printf("%d\n", linenum);
}
