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


int init_handle_SIGCHLD() {
    int result;
    struct sigaction SIGCHLD_action = {0};
    sigset_t block_mask;//, empty_mask;
                        //
    //setbuf(stdout, NULL);   /* Disable buffering of stdout */

    sigemptyset(&SIGCHLD_action.sa_mask);
    SIGCHLD_action.sa_flags = SA_RESTART;
    SIGCHLD_action.sa_handler = handle_SIGCHLD;

    result = sigaction(SIGCHLD, &SIGCHLD_action, NULL);
    if (result == -1) {
        perror("sigaction");
    } else {
        /* Block SIGCHLD to prevent its delivery if a child terminates
        before the parent commences the sigsuspend() loop in main */

        sigemptyset(&block_mask);
        sigaddset(&block_mask, SIGCHLD);

        result = sigprocmask(SIG_SETMASK, &block_mask, NULL);
        if (result == -1) {
            perror("sigprocmask");
        }
    }

    return result;
}


// Initialize a given signal handler for signal signum.
// Sets sa_mask to empty
void init_no_block(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    // Do not block any catchable signals while running
    sigemptyset(&action.sa_mask);

    // Restart any instructions interrupted by this signal
    action.sa_flags = SA_RESTART;
    sigaction(signum, &action, NULL);
}



void init_ignore(int signum) {
    init_no_block(signum, SIG_IGN);


    /* struct sigaction ignore_action  = {0}; */

    /* // The ignore_action struct has SIG_IGN as its signal handler */
    /* ignore_action.sa_handler = SIG_IGN; */

    /* // Do not block any catchable signals while running */
    /* sigemptyset(&ignore_action.sa_mask); */

    /* // */
    /* ignore_action.sa_flags = SA_RESTART; */
    /* sigaction(signum, &ignore_action, NULL); */
}








void init_parent_sig_handlers() {

    init_ignore(SIGINT);

/*     struct sigaction    SIGINT_action  = {0}; */
/* //                        SIGTSTP_action = {0}, */

/* //                        SIGTERM_action = {0}, */
/*     struct sigaction ignore_action  = {0}; */

/*     // The ignore_action struct has SIG_IGN as its signal handler */
/*     ignore_action.sa_handler = SIG_IGN; */

/*     // Fill out the SIGINT_action struct */
/*     // Register handle_SIGINT as the signal handler */
/*     SIGINT_action.sa_handler = handle_SIGINT; */

/*     // Block all catchable signals while handle_SIGINT is running */
/*     sigfillset(&SIGINT_action.sa_mask); */

/*     SIGINT_action.sa_flags = 0; */
/*     sigaction(SIGINT, &SIGINT_action, NULL); */

/*     init_handle_SIGCHLD(); */

}
// SIGTERM
// for exit clean up


void init_bg_child_sig_handlers() {

    init_ignore(SIGINT);

}


void init_fg_child_sig_handlers() {



}
