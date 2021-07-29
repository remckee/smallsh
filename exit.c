/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
**********************/

#include "smallsh.h"

void myexit(struct cmd_line *cmd_parts) {
    // kill any other processes or jobs smallsh has started
    // atexit() ?

    // free memory that may have been allocated when parsing command
    free_cmd(cmd_parts);

    exit(0);
}
