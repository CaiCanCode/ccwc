/* ccwc.c

   Coding challenges stuff. I get a link from the Linkedin page and it looked fun. I'll do more but probably not all of them
   Anyway this is to make a word/character/line count tool 

   M. Williams - 2023-12-19

   version 1.1 - 2023-12-22

   version 1.2 - 2024-02-01
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//to avoid magic constants
#define MAX_OPTIONS 4

//not strictly necessary but better for organization
typedef struct file{
    FILE* ptr;
    char* name;
}file;

//theory: don't count bytes in the form 10xxxxxx (based on UTF-8 standard)

/*
    theory: a space character (or the beggining of the file) followed by a non-spce character constitutes a word. 
    Under this definition
    "Hello World!" is 2 worda
    "Hello-World!" is 1 word
    "Hello.World!" is 1 word 
    "Hello           World!" is 2 words
    "      Hello World!" is 2 words
    "Hello World!           " is two words
    "Hello World !" is 3 words
    "Hello
    World" is 2 words (newline)
    "Hello  World!" is 2 words (tab)
*/

//Theory: the number of lines is equal to the number of newlines (could be wrong)

//order is m, l, w, c; data has size MAX_OPTIONS
void count(FILE* f, int* data){
    int char_count = 0;
    int line_count = 0;
    int word_count = 0;
    int count = 0;
    char prev = ' ';
    char ch = fgetc(f);
    while(ch != EOF){
        if(ch == '\n'){
            line_count++;        
        }
        if(isspace(prev) && !isspace(ch)){
            word_count++;
        }
        if(ch > -64){
            char_count++; 
        } 
        count++;
        prev = ch;
        ch = fgetc(f);    
    }
    *data++ = char_count;
    *data++ = line_count;
    *data++ = word_count;
    *data = count;
}

/****************************************************************************************/
//this next function is not strictly necessary but they make main cleaner

file get_filename(int argc, char** argv){
    file f;
    for(int i = 1; i < argc; i++){
        if(*(argv[i]) != '-'){
            f.name = argv[i];
            f.ptr = fopen(f.name, "r");
            return f;
        }
    }
    f.ptr = stdin;
    f.name = ""; //should be ok because this is in read only memory
    return f;
}

//buff has size MAX_OPTIONS + 1 (for the null terminator)
void get_options(int argc, char** argv, char* buff){
    int idx = 0;
    for(int i = 1; i < argc; i++){
        if(*(argv[i]) == '-'){
            buff[idx] = (argv[i])[1];
            idx++;        
        }
        if(idx >= MAX_OPTIONS){
            fprintf(stderr, "Sorry, too many options entered\n");
        }
    }
    if(!idx){
        *buff++ = 'l';
        *buff++ = 'w';
        *buff = 'c';
    }
}

int main(int argc, char** argv){
    file f = get_filename(argc, argv);
    int data[MAX_OPTIONS];
    char options[MAX_OPTIONS + 1] = {0};
    count(f.ptr, data);
    get_options(argc, argv, options);
    if(strchr(options, 'm')){
        printf("%d ", *data);
    }
    if(strchr(options, 'l')){
        printf("%d ", data[1]);
    }
    if(strchr(options, 'w')){
        printf("%d ", data[2]);
    }
    if(strchr(options, 'c')){
        printf("%d ", data[3]);
    }
    printf("%s\n", f.name);
    return 0;
}
