//parseline.c
//Purpose: Implementation of parseline; parse the given string to the
//         desired line format given by the spec.
//Authors: Tianyi Ma, Alexander Tsueaki
//Date Modified: 9/14/19

#include "parseline.h"
#include <stdlib.h>

//parseline
//arguments:
//           s: a pointer to a string to be parsed
//           size: size of line being passed in
//returns: a bool representing if the string in the line passed in is is 'empty'
//         empty refers to only having a '\n' after being parsed.
//purpose: to parse a string into a uniform format such that words are 
//         separated by one space (ends on a newline/eof) 
//         
bool parseline(char** s, size_t len) {
    size_t c_ct = 0;
    bool is_word = false;
    char c;
    for (size_t i = 0; i < len; i++) {
        c = (*s)[i];
        if (is_char(c) == true) {
            if (is_word == false) {
                is_word = true;
            }
            (*s)[c_ct] = c;
            c_ct++;
        }
        else if (is_word == true) {
            is_word = false;
            (*s)[c_ct] = ' ';
            c_ct++;
        }
    }
    if (c_ct > 0 && (!(c_ct == 1 && c == '\n'))) {
        size_t new_size;
        if (is_word == true) new_size = c_ct + 1;
        else new_size = c_ct;
        *s = realloc(*s, new_size * sizeof(char));
        (*s)[new_size - 1] = '\0';
        return true;
    }
    else {
        return false;
    }
}

//is_char
//arguments:
//          c: a char to be validated
//returns: a bool representing if a char passed in is a valid word character
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