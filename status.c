/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


int get_status(int wstatus, char *type) {
    int status;

    if(WIFEXITED(wstatus)){
        *type = EXIT;
        status = WEXITSTATUS(wstatus);

    } else if(WIFSIGNALED(wstatus)) {
        *type = TERM;
        status = WTERMSIG(wstatus);

    }
    return status;
}


void report_status(int status, char type) {
    char *message;
    //size_t size = 0;

    if (type==TERM) {
        message = "terminated by signal";
    } else {
        message = "exit value";
    }

    printf("%s %d\n", message, status);
}


