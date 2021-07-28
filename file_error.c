/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/27/2021
**********************/

#include "smallsh.h"

/* This function first checks whether there was an error resulting from
 * a function call. The function that was called (function) must take a
 * file descriptor or file name corresponding to the file file_name as an
 * argument. If there was an error, it reports the error and terminates
 * the program. It checks for an error by comparing the return value
 * (returned) of function to error_value.
 */
void exit_if_error(bool condition, char *file_name, char *function) {
    if (condition) {
        fprintf(stderr, "%s() failed on \"%s\"\n", function, file_name);
        exit(1);
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
    }
}

