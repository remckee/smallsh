#include "smallsh.h"


/**
 * exit_if_error():
 * Exit the current process with an error message if @condition is true.
 * @condition: the condition to check
 * @msg: the error message to display
 */
void exit_if_error(bool condition, char *msg) {
    if (condition) {
        perror(msg);
        fflush(stdout);
        exit(FAILURE);
    }
}


/**
 * bg_exit_if_error():
 * Exit the current process if @condition is true, but don't display a message.
 * @condition: the condition to check
 */
void bg_exit_if_error(bool condition) {
    if (condition) {
        exit(FAILURE);
    }
}


/**
 * The following functions display a warning message about specific error
 * if @condition evaluates to true, but they do not exit. These are preferable
 * in portions of code that are only run by the parent process to avoid
 * exiting smallsh. For the caller's convenience, these functions return
 * @condition.
 */


/**
 * warn_error():
 * If @condition is true, display a message.
 * @condition: the condition to check
 * @msg: the warning message to display
 *
 * Return:
 * @condition, unmodified
 */
bool warn_error(bool condition, char *msg) {
    if (condition) {
        perror(msg);
        fflush(stdout);
    }
    return condition;
}


/**
 * warn_dne():
 * If @condition is true, display a message that @file_name does not exist.
 * @condition: the condition to check
 * @program: the name of the program
 * @file_name: the name of the file
 *
 * Return:
 * @condition, unmodified
 */
bool warn_dne(bool condition, char *program, char *file_name) {
    if (condition) {
        fprintf(stderr, "%s: %s: No such file or directory\n", program, file_name);
        fflush(stdout);
    }
    return condition;
}


/**
 * warn_chars():
 * If @condition is true, display a message that the command line exceeded
 * the character limit.
 * @condition: the condition to check
 * @max_chars: the character limit
 *
 * Return:
 * @condition, unmodified
 */
bool warn_chars(bool condition, int max_chars) {
    if (condition) {
        printf("The command line was too long. A line can have a maximum of %d characters.\n", max_chars);
        fflush(stdout);
    }
    return condition;
}


/**
 * warn_args():
 * If @condition is true, display a message that the command line exceeded
 * the argument limit.
 * @condition: the condition to check
 * @max_args: the argument limit
 *
 * Return:
 * @condition, unmodified
 */
bool warn_args(bool condition, int max_args) {
    if (condition) {
        printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", max_args);
        fflush(stdout);
    }
    return condition;
}

