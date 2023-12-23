/* ccwc.c

   Coding challenges stuff. I get a link from the Linkedin page and it looked fun. I'll do more but probably not all of them
   Anyway this is to make a word/character/line count tool 

   M. Williams - 2023-12-19

   version 1.1 - 2023-12-22
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//not strictly necessary but better for organization
typedef struct file{
    FILE* ptr;
    char* name;
}file;

int character_count(FILE* f){
    int count = 0;
    while(fgetc(f) != EOF){
        count++;    
    }
    return count;
}

//theory: don't count bytes in the form 10xxxxxx (based on UTF-8 standard)
int multibyte_character_count(FILE* f){
    int count = 0;
    char ch = fgetc(f);
    while(ch != EOF){
        if(ch > -64){
            count++; 
        }   
        ch = fgetc(f);
    }
    return count;
}

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
int word_count(FILE* f){
    int count = 0;
    char prev = ' ';
    char cur = fgetc(f);
    while(cur != EOF){
        if(isspace(prev) && !isspace(cur)){
            count++;        
        }
        prev = cur;
        cur = fgetc(f);
    }
    return count;
}

//Theory: the number of lines is equal to the number of newlines (could be wrong)
int line_count(FILE* f){
    int count = 0;
    char ch = fgetc(f);
    while(ch != EOF){
        if(ch == '\n'){
            count++;        
        }
        ch = fgetc(f);    
    }
    return count;
}

/****************************************************************************************/
//these next two functions are not strictly necessary but they make main cleaner

file get_filename(int argc, char** argv){
    file f;
    int idx = 2;
    switch (argc){
        case 1: //break statement not needed due to return
            f.ptr = stdin;
            f.name = "";
            return f;
        case 2: //break statement not needed because why write the same code
            if(*(argv[1]) == '-'){
                f.ptr = stdin;
                f.name = "";
                return f;
            }
            idx = 1;
        case 3:
            f.ptr = fopen(argv[idx], "r");
            f.name = argv[idx];
            break;
        default:
            fprintf(stderr, "Sorry, we only support 0 or 1 arguments and 0 or 1 options at this time. If I haven't written a manual entry yet I'll definitely do so later.\n");
            exit(2);
    }
    if(!f.ptr){
        fprintf(stderr, "couldn't open file\n");
        exit(1);
    }
    return f;
}

char get_option(int argc, char** argv){
    if(argc > 1 && *(argv[1]) == '-'){
        return (argv[1])[1];
    }
    return '/0';
}

file copy(file* f){
    file temp;
    temp.name = f->name;
    temp.ptr = tmpfile();
    if(!temp.ptr){
        fprintf(stderr, "Error creating temporary file\n");
        return *f;
    }
    char ch = fgetc(f->ptr);
    while(ch != EOF){
        fputc(ch, temp.ptr);
        ch = fgetc(f->ptr);
    }
    fclose(f->ptr);
    rewind(temp.ptr);
    return temp;
}

int main(int argc, char** argv){
    file f = get_filename(argc, argv);
    char str[300] = ""; //probably big enough
    char buff[30]; //definitely big enough
    int stop = 1;
    switch (get_option(argc, argv)){
        case 'm':
            sprintf(buff, "%d", multibyte_character_count(f.ptr));
            strcat(str, buff);
            strcat(str, " ");
            break;
        case '/0': //don't break
            stop = 0;
            
            f = (f.ptr==stdin) ? copy(&f) : f;
            if(f.ptr == stdin){
                return 1;
            }
        case 'l':
            sprintf(buff, "%d", line_count(f.ptr));
            strcat(str, buff);
            strcat(str, " ");
            if(stop){
                break;
            }else{
                rewind(f.ptr);
            }
        case 'w':
            sprintf(buff, "%d", word_count(f.ptr));
            strcat(str, buff);
            strcat(str, " ");
            if(stop){
                break;
            }else{
                rewind(f.ptr);
            }
        case 'c':
            sprintf(buff, "%d", character_count(f.ptr));
            strcat(str, buff);
            strcat(str, " ");
            break;
        default:
            fprintf(stderr, "sorry, I don't know what to count. Use -c for bytes, -l for lines, -w for words, -m for charaters, and do not specify to print the first three.\n");
            return 3;
    }
    strcat(str, f.name);
    if(f.ptr != stdin){
        fclose(f.ptr);
    }
    printf("%s\n", str);
    return 0;
}

// activate with "gcc -Wall -std=c18 -o name name.c"
