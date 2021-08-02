/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
**********************/

#include "smallsh.h"


// Writes num to stdout using only nonreentrant functions.
// returns a positive value on success
// returns a negative value on error
ssize_t write_number(long num) {
    int num_len = 21;  // 9223372036854775807 has 19 chars + 2 for sign and \0
    char num_ascii[num_len];

    // convert num to an ascii string and store in num_ascii
    num_len = ltoa_dec_buf(num, num_ascii, num_len);

    if (num_len > 0) {
        num_len = write(STDOUT_FILENO, &num_ascii, num_len);
        fflush(stdout);
    }

    // reassign a result of 0 to -1 to indicate an error
    num_len = (num_len==0) ? -1 : num_len;

    return num_len;
}


// SIGINT (CTRL-C)
// parent, child background: ignore
// child foreground: terminate itself
void handle_SIGINT(int signum) {

    static char msg[] = "fg received SIGINT\n";
    write(STDOUT_FILENO, msg, 19);
    fflush(stdout);
    if (signum == SIGINT) {
        exit(FAILURE);
    }
}


// SIGTSTP (CTRL-Z)
// child foreground or background: ignore
// parent: enter or exit foreground-only mode
void handle_SIGTSTP(int signum) {

    if (!get_fg_only()) {
        static char msg[] = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, msg, 51);
        fflush(stdout);
    } else {
        static char msg[] = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, msg, 31);
        fflush(stdout);
    }

    toggle_fg_only();
}


void pause_SIGTSTP(int signum) {
    /* sigset_t block_mask; */


    /* int result = sigprocmask(SIG_SETMASK, &block_mask, NULL); */
    /* if (result == -1) { */
    /*     perror("sigprocmask"); */
    /* } */

    pause();
    //handle_SIGTSTP(signum);
}






// Based on The Linux Programming Interface: Chapter 26, Listing 26-5
//
// for notifying parent that child background process is complete
/* void handle_SIGCHLD(int signum) { */
/*     int status, saved_errno; */
/*     pid_t pid; */
/*     int status_type; */

/*     saved_errno = errno; */

/*     while((pid = waitpid(-1, &status, WNOHANG)) > 0){ */
/*         // report child pid when background process finishes */
/*         write(STDOUT_FILENO, "background pid %d is done: ", 50); //, pid */
/*         fflush(stdout); */

/*         status = get_status(status, &status_type); */
/*         report_status(status, status_type); */

/*     } */

/*     if (pid == -1 && errno != ECHILD) { */
/*         // errMsg("waitpid"); */
/*     } */
/*     errno = saved_errno; */
/* } */


// https://man7.org/linux/man-pages/man3/wait.3p.html#EXAMPLES
void handle_sigchld(int signum) {
    int sav_errno = errno;

    static char msg4[] = "starting\n";
    write(STDOUT_FILENO, msg4, 15);
    fflush(stdout);

    siginfo_t info;
    memset(&info, 0, sizeof(siginfo_t));
    //int wstatus;
    //int type;
    pid_t pid;
    bool remaining = true;
    /*
    * Obtain status information for the child which
    * caused the SIGCHLD signal and write its exit code
    * to stdout.
    */
    int result;// = waitid(P_ALL, 0, &info, WEXITED | WNOHANG);

    while ((result = waitid(P_ALL, 0, &info, WEXITED | WNOHANG)) != -1
           && remaining) {
        if (info.si_pid == 0) {
            /* No children changed state */
            static char msg1[] = "background nod ";
            write(STDOUT_FILENO, msg1, 15);
            fflush(stdout);
            remaining = false;
        } else {
            pid = info.si_pid;

            // report child pid when background process finishes
            static char msg1[] = "background pid ";
            write(STDOUT_FILENO, msg1, 15);
            fflush(stdout);

            int num_written = write_number(pid);

            if (num_written == -1) {
                static char msg[] = "write_number() failed\n";
                write(STDOUT_FILENO, msg, 22);
                fflush(stdout);
            } else {
                static char msg2[] = " is done: ";
                write(STDOUT_FILENO, msg2, 10);
                fflush(stdout);

                //int status_num = get_status(wstatus,  &type);
                //report_status(status_num, type);
                //
                int status = info.si_status; //get_status(*status, status_type);
                int status_type = info.si_code;
                report_status(status, status_type);
            }
        }
    }

    if (result == -1) {
        perror("waitid");
        fflush(stdout);
    }
    /* if (sinfo->si_code == CLD_EXITED) { */
    /*     static char msg[] = "wrong si_code\n"; */
    /*     write(STDOUT_FILENO, msg, sizeof msg - 1); */
    /*     fflush(stdout); */

    /* } else if (waitpid(pid, &wstatus, 0) == -1) { */
    /*     static char msg[] = "waitpid() failed\n"; */
    /*     write(STDOUT_FILENO, msg, 17); */
    /*     fflush(stdout); */

    /* } else { */
    /*     // report child pid when background process finishes */
    /*     static char msg1[] = "background pid "; */
    /*     write(STDOUT_FILENO, msg1, 15); */
    /*     fflush(stdout); */

    /*     int result = write_number(pid); */

    /*     if (result == -1) { */
    /*         static char msg[] = "write_number() failed\n"; */
    /*         write(STDOUT_FILENO, msg, 22); */
    /*         fflush(stdout); */
    /*     } else { */
    /*         static char msg2[] = " is done: "; */
    /*         write(STDOUT_FILENO, msg2, 10); */
    /*         fflush(stdout); */

    /*         int status_num = get_status(wstatus,  &type); */
    /*         report_status(status_num, type); */
    /*     } */
    /* } */
    errno = sav_errno;
}


int init_handle_SIGCHLD() {
    struct sigaction action = {0};

    action.sa_flags = SA_RESTART;
    action.sa_handler = handle_sigchld;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGCHLD);

    if (sigaction(SIGCHLD, &action, NULL) == -1) {
        perror("sigaction");
        exit(FAILURE);
    } else {
        printf("init_handle_SIGCHLD\n");
        fflush(stdout);
    }


    return 0;
    /* int result; */
    /* struct sigaction SIGCHLD_action = {0}; */
    /* sigset_t block_mask;//, empty_mask; */
    /*                     // */
    /* //setbuf(stdout, NULL);    */ /* Disable buffering of stdout */

    /* sigemptyset(&SIGCHLD_action.sa_mask); */
    /* SIGCHLD_action.sa_flags = SA_RESTART | SA_NOCLDSTOP; */
    /* SIGCHLD_action.sa_handler = handle_SIGCHLD; */

    /* result = sigaction(SIGCHLD, &SIGCHLD_action, NULL); */
    /* if (result == -1) { */
    /*     perror("sigaction"); */
    /* } else { */
        /* Block SIGCHLD to prevent its delivery if a child terminates */
    /*     before the parent commences the sigsuspend() loop in main */

    /*     sigemptyset(&block_mask); */
    /*     sigaddset(&block_mask, SIGCHLD); */

    /*     result = sigprocmask(SIG_SETMASK, &block_mask, NULL); */
    /*     if (result == -1) { */
    /*         perror("sigprocmask"); */
    /*     } */
    /* } */

    /* return result; */
}


// Initialize a given signal handler for signal signum.
// Sets sa_mask to have all signals
int init_block_all(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    // Block all catchable signals while running
    sigfillset(&action.sa_mask);

    // Restart any instructions interrupted by this signal
    action.sa_flags = SA_RESTART;
    return sigaction(signum, &action, NULL);
}


// Initialize a given signal handler for signal signum.
// Sets sa_mask to have all signals
int init_block_all_no_restart(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    // Block all catchable signals while running
    sigfillset(&action.sa_mask);

    // no sa flags
    action.sa_flags = 0;
    return sigaction(signum, &action, NULL);
}


// Initialize a given signal handler for signal signum.
// Sets sa_mask to empty
int init_no_block(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    // Do not block any catchable signals while running
    sigemptyset(&action.sa_mask);

    // Restart any instructions interrupted by this signal
    action.sa_flags = SA_RESTART;
    return sigaction(signum, &action, NULL);
}


// Initialize a given signal handler for signal signum.
// Sets sa_mask to empty
int init_no_block_no_restart(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    // Do not block any catchable signals while running
    sigemptyset(&action.sa_mask);

    // no sa flags
    action.sa_flags = 0;
    return sigaction(signum, &action, NULL);
}


int init_ignore(int signum) {
    return init_no_block(signum, SIG_IGN);
}


int init_ignore_block(int signum) {
    return init_block_all(signum, SIG_IGN);
}


int init_ignore_block_no_restart(int signum) {
    return init_block_all_no_restart(signum, SIG_IGN);
}


int init_pause_SIGTSTP() {
    return init_no_block_no_restart(SIGTSTP, pause_SIGTSTP);
}


int init_handle_SIGTSTP() {
    return init_no_block_no_restart(SIGTSTP, handle_SIGTSTP);
}


void init_parent_sig_handlers() {

    init_ignore(SIGINT);
    init_no_block_no_restart(SIGTSTP, handle_SIGTSTP);
    init_handle_SIGCHLD();
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
/* void handle_fg_SIGTSTP(int signum) { */

/*     if (signum == SIGINT) { */
/*         exit(FAILURE); */
/*     } */
/* } */



void init_fg_child_sig_handlers() {

    init_block_all_no_restart(SIGINT, SIG_DFL); //default : SIG_DFL
    init_ignore(SIGTSTP);

}


void init_bg_child_sig_handlers() {

    init_ignore(SIGINT);
    init_ignore(SIGTSTP);
}

