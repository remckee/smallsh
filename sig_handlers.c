/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
**********************/

#include "smallsh.h"


void init_sig_handlers() {
    struct sigaction    SIGINT_action  = {0};
//                        SIGTSTP_action = {0},
//                        SIGCHLD_action = {0},
//                        SIGTERM_action = {0},
//                        ignore_action  = {0};

    // Fill out the SIGINT_action struct
    // Register handle_SIGINT as the signal handler
    SIGINT_action.sa_handler = handle_SIGINT;

    // Block all catchable signals while handle_SIGINT is running
    sigfillset(&SIGINT_action.sa_mask);

    SIGINT_action.sa_flags = 0;
    sigaction(SIGINT, &SIGINT_action, NULL);



}



// SIGINT (CTRL-C)
// parent, child background: ignore
// child foreground: terminate itself
void handle_SIGINT(int signum) {

}


// SIGTSTP (CTRL-Z)
// child foreground or background: ignore
// parent: enter or exit foreground-only mode



// SIGCHLD
// for notifying parent that child background process is complete



// SIGTERM
// for exit clean up

