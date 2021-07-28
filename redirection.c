/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


int redirect(char *file_name, char *msg, int flags, int new_fd) {
    int success = -1;
    int fd = open(file_name, flags, S_IRW);

    if (fd == -1) {
        success = fd;
        fprintf(stderr, "cannot open %s for %s\n", file_name, msg);

    } else {
        success = dup2(fd, new_fd);
    }
    return success;
}


int redirect_input(char *file_name) {
    /* int success = -1; */
    /* int input_fd = open(file_name, O_RDONLY, S_IRW); */

    /* if (input_fd == -1) { */
    /*     success = input_fd; */
    /*     fprintf(stderr, "cannot open %s for input\n", file_name); */

    /* } else { */
    /*     success = dup2(input_fd, STDIN_FILENO); */
    /* } */
    /* return success; */

    return redirect(file_name, "input", O_RDONLY, STDIN_FILENO);
}



int redirect_output(char *file_name) {
    /* int success = -1; */
    /* int output_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRW); */

    /* if (output_fd == -1) { */
    /*     success = output_fd; */
    /*     fprintf(stderr, "cannot open %s for output\n", file_name); */

    /* } else { */
    /*     success = dup2(output_fd, STDOUT_FILENO); */
    /* } */
    /* return success; */
    return redirect(file_name, "output", O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
}
