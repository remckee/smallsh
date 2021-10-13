#include "smallsh.h"


// Exits the current process with an error message if the given condition is true.
void exit_if_error(bool condition, char *msg) {
    if (condition) {
        perror(msg);
        fflush(stdout);
        exit(FAILURE);
    }
}


// Exits the current process if the given condition is true.
void bg_exit_if_error(bool condition) {
    if (condition) {
        exit(FAILURE);
    }
}


/*
 * The following functions display a warning message about specific error
 * if condition evaluates to true, but they do not exit. These are preferable
 * in portions of code that are only run by the parent process to avoid
 * exiting smallsh.
 */


// Error message is specified by msg.
// Returns the value of the condition for the caller's convenience (e.g.,
// so it can be used in an if statement).
bool warn_error(bool condition, char *msg) {
    if (condition) {
        perror(msg);
        fflush(stdout);
    }
    return condition;
}


// file or directory does not exist
// Returns the value of the condition for the caller's convenience.
bool warn_dne(bool condition, char *program, char *file_name) {
    if (condition) {
        fprintf(stderr, "%s: %s: No such file or directory\n", program, file_name);
        fflush(stdout);
    }
    return condition;
}


// The command line exceeded the character limit.
// Returns the value of the condition for the caller's convenience.
bool warn_chars(bool condition, int max_chars) {
    if (condition) {
        printf("The command line was too long. A line can have a maximum of %d characters.\n", max_chars);
        fflush(stdout);
    }
    return condition;
}


// The command line exceeded the argument limit.
// Returns the value of the condition for the caller's convenience.
bool warn_args(bool condition, int max_args) {
    if (condition) {
        printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", max_args);
        fflush(stdout);
    }
    return condition;
}

