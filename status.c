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
    size_t msg_len = 0;
    //int status_len = num_digits(status);
    //char status_ascii[status_len+1];
    char nl = '\n';

    // convert status to an ascii string and store in status_ascii
    //ltoa_dec_buf(status, status_ascii, sizeof(status_ascii));

    if (type==TERM) {
        message = "terminated by signal ";
        msg_len = 22;
    } else {
        message = "exit value ";
        msg_len = 12;
    }

    //printf("%s %d\n", message, status);
    write(STDOUT_FILENO, message, msg_len);
    //printf("%d", status);
    write_number(status);
    write(STDOUT_FILENO, &nl, 2);
}


