/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
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


void free_cmd(struct cmd_line *cmd_parts) {
    // free memory that may have been allocated to expand $$ variables
    free_safe(cmd_parts->cmd);
    free_safe(cmd_parts->input_file);
    free_safe(cmd_parts->output_file);

    // Start at 1 because cmd_parts->args[0] is a pointer to cmd_parts->cmd,
    // which was already freed.
    for (int i = 1; i < cmd_parts->argsc && cmd_parts->args[i]; i++) {
        free_safe(cmd_parts->args[i]);
    }

    // free memory allocated in get_cmd
    free_safe(cmd_parts);
}

