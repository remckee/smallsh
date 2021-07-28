/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/27/2021
**********************/

#include "smallsh.h"

// Changes current working directory to a named directory or HOME
int mycd(char *args[], int argsc) {
    int success = -1;

    if (argsc > 1) {
        // change to named directory if an argument was provided
        success = chdir(args[1]);

    } else if (args[0]) {
        // change to HOME directory
        success = chdir(getenv("HOME"));

        // assert if there is an error changing to HOME directory
        assert(success != -1);
    }
    return success;
}
