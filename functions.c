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
