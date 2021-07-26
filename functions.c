/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/26/2021
**********************/

#include "smallsh.h"


void *malloc_safe(void *ptr, size_t size) {
    void *loc_ptr = malloc(size);

    if(loc_ptr) {
        ptr = loc_ptr;
    } else {

    }
    return ptr;
}

void free_safe(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

char *print_string_safe(char *str) {
    return (str) ? str : "NULL";
}

bool warn_chars(bool condition, int max_chars) {
    bool result = false;
    if (condition) {
        printf("The command line was too long. A line can have a maximum of %d characters.\n", max_chars);
        fflush(NULL);
        result = true;
    }
    return result;
}

bool warn_args(bool condition, int max_args) {
    bool result = false;
    if (condition) {
        printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", max_args);
        fflush(NULL);
        result = true;
    }
    return result;
}
