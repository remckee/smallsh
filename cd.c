/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/27/2021
**********************/

#include "smallsh.h"

// Changes current working directory to a named directory or HOME
int mycd(char *cmd, char *args[], int argsc) {
    int success;

    if (argsc > 0) {
        // change to named directory if an argument was provided
        success = chdir(args[0]);

    } else if (cmd) {
        // change to HOME directory
        success = chdir(getenv("HOME"));
    }
    return success;
}
