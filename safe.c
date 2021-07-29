/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


void *malloc_safe(void *ptr, size_t size) {
    void *loc_ptr = malloc(size);

    if(loc_ptr) {
        ptr = loc_ptr;
    } else {
        assert(loc_ptr!=NULL);
    }
    return ptr;
}


// free_safe will check whether ptr is NULL, but it will not check
// whether ptr was actually allocated.
void free_safe(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}


char *print_string_safe(char *str) {
    return (str) ? str : "NULL";
}

