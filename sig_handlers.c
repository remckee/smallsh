#include "smallsh.h"


// SIGTSTP (CTRL-Z) signal handler
// parent: enter or exit foreground-only mode
// Uses the functions get_fg_only() and toggle_fg_only() to indirectly
// access the file scoped variable in fg_only_mode.c.
void handle_sigtstp(int signum) {
    // display message depending on fg_only status
    if (!get_fg_only()) {
        static char msg[] = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, msg, 51);
        fflush(stdout);
    } else {
        static char msg[] = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, msg, 31);
        fflush(stdout);
    }

    // change fg_only status
    toggle_fg_only();
}


// Initialize a given signal handler for signal signum.
// Sets sa_mask to block all signals
// Returns -1 if an error occurred.
int init_handle_sigint(int signum, void (*handler)(int)) {
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
// Returns -1 if an error occurred.
int init_handle_sigtstp(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    // Do not block any catchable signals while running
    sigemptyset(&action.sa_mask);

    // no sa flags
    action.sa_flags = 0;
    return sigaction(signum, &action, NULL);
}


// Initialize SIG_IGN for signal signum.
// Returns -1 if an error occurred.
int init_ignore(int signum) {
    struct sigaction action  = {0};
    action.sa_handler = SIG_IGN;

    // Do not block any catchable signals while running
    sigemptyset(&action.sa_mask);

    // Restart any instructions interrupted by this signal
    action.sa_flags = SA_RESTART;
    return sigaction(signum, &action, NULL);
}


// Initializes the signal handlers for the parent process. Errors here are
// fatal (e.g., exit the shell) because they likely indicate an error
// related to the signal handler or specified signum being invalid.
void init_parent_sig_handlers() {
    exit_if_error(init_ignore(SIGINT)==-1, "sigaction");
    exit_if_error(init_handle_sigtstp(SIGTSTP, handle_sigtstp)==-1, "sigaction");
}


// Initializes the signal handlers for a fg child process.
void init_fg_child_sig_handlers() {
    exit_if_error(init_handle_sigint(SIGINT, SIG_DFL)==-1, "sigaction");
    exit_if_error(init_ignore(SIGTSTP)==-1, "sigaction");
}


// Initializes the signal handlers for a bg child process.
void init_bg_child_sig_handlers() {
    exit_if_error(init_ignore(SIGINT)==-1, "sigaction");
    exit_if_error(init_ignore(SIGTSTP)==-1, "sigaction");
}

