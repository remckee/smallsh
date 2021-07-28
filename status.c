/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


int get_status(int wstatus, char *type) {
    int status_val;

    if(WIFEXITED(wstatus)){
        *type = EXIT;
        status_val = WEXITSTATUS(wstatus);

    } else if(WIFSIGNALED(wstatus)) {
        *type = TERM;
        status_val = WTERMSIG(wstatus);

    }
    return status_val;
}


void report_status(int val, char type) {
    char *message;
    //size_t size = 0;

    if (type==TERM) {
        message = "terminated by signal";
    } else {
        message = "exit value";
    }

    printf("%s %d\n", message, val);
}


