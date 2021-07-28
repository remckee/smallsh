/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


int redirect_input(char *file_name) {
    int success = -1;
    int input_fd = open(file_name, O_RDONLY, S_IRW);

    if (input_fd == -1) {
        success = input_fd;
        fprintf(stderr, "cannot open %s for input\n", file_name);

    } else {
        success = dup2(input_fd, STDIN_FILENO);
    }
    return success;
}
