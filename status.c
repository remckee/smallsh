/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/27/2021
**********************/

#include "smallsh.h"


int mystatus(int status) {

    return status;
}


void report_status(int status, char type) {
    char *message;
    //size_t size = 0;

    if (type=='t') {
        message = "terminated by signal";
    } else {
        message = "exit value";
    }

    printf("%s %d\n", message, status);
}
