/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
**********************/

#include "smallsh.h"






// SIGINT (CTRL-C)
// parent, child background: ignore
// child foreground: terminate itself
void handle_SIGINT(int signum) {

}


// SIGTSTP (CTRL-Z)
// child foreground or background: ignore
// parent: enter or exit foreground-only mode


// Based on The Linux Programming Interface: Chapter 26, Listing 26-5
//
// for notifying parent that child background process is complete
void handle_SIGCHLD(int signum) {
    int status, saved_errno;
    pid_t pid;
    char status_type;

    saved_errno = errno;

    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        // report child pid when background process finishes
        write(STDOUT_FILENO, "background pid %d is done: ", 50); //, pid
        fflush(stdout);

        status = get_status(status, &status_type);
        report_status(status, status_type);

    }

    if (pid == -1 && errno != ECHILD) {
        // errMsg("waitpid");
    }
    errno = saved_errno;
}


void init_handle_SIGCHLD() {
    struct sigaction SIGCHLD_action = {0};
    sigset_t block_mask;//, empty_mask;
                        //
    setbuf(stdout, NULL);   /* Disable buffering of stdout */

    sigemptyset(&SIGCHLD_action.sa_mask);
    SIGCHLD_action.sa_flags = 0;
    SIGCHLD_action.sa_handler = handle_SIGCHLD;
    if (sigaction(SIGCHLD, &SIGCHLD_action, NULL) == -1) {
        perror("sigaction");
    }

    /* Block SIGCHLD to prevent its delivery if a child terminates
    before the parent commences the sigsuspend() loop below */

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);

    if (sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1) {
        // errExit("sigprocmask");
    }
}




void init_sig_handlers() {
    struct sigaction    SIGINT_action  = {0};
//                        SIGTSTP_action = {0},

//                        SIGTERM_action = {0},
//                        ignore_action  = {0};


    // Fill out the SIGINT_action struct
    // Register handle_SIGINT as the signal handler
    SIGINT_action.sa_handler = handle_SIGINT;

    // Block all catchable signals while handle_SIGINT is running
    sigfillset(&SIGINT_action.sa_mask);

    SIGINT_action.sa_flags = 0;
    sigaction(SIGINT, &SIGINT_action, NULL);

    init_handle_SIGCHLD();

}
// SIGTERM
// for exit clean up

