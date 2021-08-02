/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
**********************/

#include "smallsh.h"

/* This function first checks whether there was an error resulting from*/
/*  * a function call. The function that was called (function) must take a */
/*  * file descriptor or file name corresponding to the file file_name as an */
/*  * argument. If there was an error, it reports the error and terminates */
/*  * the program. It checks for an error by comparing the return value */
/*  * (returned) of function to error_value. */
/* void exit_if_error(bool condition, char *file_name, char *function) { */
/*     if (condition) { */
/*         fprintf(stderr, "%s() failed on \"%s\"\n", function, file_name); */
/*         fflush(stdout); */
/*         exit(1); */
/*     } */
/* } */

void fg_exit_if_error(struct cmd_line *cmd_parts, bool condition, char *msg,
                      int *status, int *status_type) {
    if (condition) {
        perror(msg);
        fflush(stdout);
        *status = FAILURE;
        *status_type = CLD_EXITED;
        myexit(cmd_parts, *status);
    }
}


void bg_exit_if_error(struct cmd_line *cmd_parts, bool condition) {
    if (condition) {
        int status = FAILURE;
        int status_type = CLD_EXITED;
        report_status(status, status_type);
        myexit(cmd_parts, status);
    }
}


/*
 * The following functions display a warning message about specific error
 * if condition evaluates to true.
 */

// file or directory does not exist
void warn_dne(bool condition, char *program, char *file_name) {
    if (condition) {
        fprintf(stderr, "%s: %s: No such file or directory\n", program, file_name);
        fflush(stdout);
    }
}


bool warn_chars(bool condition, int max_chars) {
    if (condition) {
        printf("The command line was too long. A line can have a maximum of %d characters.\n", max_chars);
        fflush(stdout);
    }
    return condition;
}


bool warn_args(bool condition, int max_args) {
    if (condition) {
        printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", max_args);
        fflush(stdout);
    }
    return condition;
}

