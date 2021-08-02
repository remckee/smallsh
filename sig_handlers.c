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


// SIGTSTP (CTRL-Z)
// child foreground or background: ignore
// parent: enter or exit foreground-only mode
void handle_sigtstp(int signum) {
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


// Based on The Linux Programming Interface: Chapter 26, Listing 26-5
// and https://man7.org/linux/man-pages/man3/wait.3p.html#EXAMPLES
// for notifying parent that child background process is complete
void handle_sigchld(int signum) {
    int sav_errno = errno;
    siginfo_t info;
    memset(&info, 0, sizeof(siginfo_t));
    pid_t pid;
    bool remaining = true;
    int result;

    while ((result = waitid(P_ALL, 0, &info, WEXITED | WNOHANG)) != -1
           && remaining) {
        if (info.si_pid == 0) {
            /* No children changed state */
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

                int status = info.si_status;
                int status_type = info.si_code;
                report_status(status, status_type);
            }
        }
    }

    if (result == -1) {
        perror("waitid");
        fflush(stdout);
    }
    errno = sav_errno;
}


int init_handle_sigchld() {
    struct sigaction action = {0};

    action.sa_flags = SA_RESTART;
    action.sa_handler = handle_sigchld;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGCHLD);

    if (sigaction(SIGCHLD, &action, NULL) == -1) {
        perror("sigaction");
        exit(FAILURE);
    }
    return 0;
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


/* int init_ignore_block(int signum) { */
/*     return init_block_all(signum, SIG_IGN); */
/* } */


/* int init_ignore_block_no_restart(int signum) { */
/*     return init_block_all_no_restart(signum, SIG_IGN); */
/* } */


void init_parent_sig_handlers() {
    init_ignore(SIGINT);
    init_no_block_no_restart(SIGTSTP, handle_sigtstp);
    init_handle_sigchld();
}


void init_fg_child_sig_handlers() {
    init_block_all_no_restart(SIGINT, SIG_DFL);
    init_ignore(SIGTSTP);
}


void init_bg_child_sig_handlers() {
    init_ignore(SIGINT);
    init_ignore(SIGTSTP);
}

