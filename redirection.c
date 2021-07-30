/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
**********************/

#include "smallsh.h"


int redirect(struct cmd_line *cmd_parts, char *file_name, char *msg, int flags, int new_fd) {
    int fd = open(file_name, flags, S_IRW);

    if (fd == -1) {
        fprintf(stderr, "cannot open %s for %s\n", file_name, msg);
        fflush(stdout);
        myexit(cmd_parts, FAILURE);
    } else {
        fd = dup2(fd, new_fd);

        if (fd == -1) {
            fprintf(stderr, "cannot redirect %s to %s\n", msg, file_name);
            fflush(stdout);
            myexit(cmd_parts, FAILURE);
        }
    }
    return fd;
}


int redirect_input(struct cmd_line *cmd_parts, char *file_name) {
    return redirect(cmd_parts, file_name, "input",
                    O_RDONLY, STDIN_FILENO);
}


int redirect_output(struct cmd_line *cmd_parts, char *file_name) {
    return redirect(cmd_parts, file_name, "output",
                    O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
}
