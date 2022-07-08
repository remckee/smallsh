#include "smallsh.h"


/**
 * handle_sigtstp():
 * SIGTSTP (CTRL-Z) signal handler
 * parent: enter or exit foreground-only mode
 * @signum: number of the signal
 */
void handle_sigtstp(int signum) {
    /* display message depending on fg_only status */
    if (!get_fg_only()) {
        static char msg[] = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, msg, 51);
        fflush(stdout);
    } else {
        static char msg[] = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, msg, 31);
        fflush(stdout);
    }

    /* change fg_only status */
    toggle_fg_only();
}


/**
 * init_handle_sigint():
 * Initialize a signal @handler for signal @signum.
 * Sets sa_mask to block all signals
 * @signum: number of the signal
 * @handler: function pointer for signal handler to initialize
 *
 * Returns:
 * -1 if an error occurred
 */
int init_handle_sigint(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    /* Block all catchable signals while running */
    sigfillset(&action.sa_mask);

    action.sa_flags = 0;
    return sigaction(signum, &action, NULL);
}


/**
 * init_handle_sigtstp():
 * Initialize a signal @handler for signal @signum.
 * Sets sa_mask to empty
 * @signum: number of the signal
 * @handler: function pointer for signal handler to initialize
 *
 * Returns:
 * -1 if an error occurred
 */
int init_handle_sigtstp(int signum, void (*handler)(int)) {
    struct sigaction action  = {0};
    action.sa_handler = handler;

    /* Do not block any catchable signals while running */
    sigemptyset(&action.sa_mask);

    action.sa_flags = 0;
    return sigaction(signum, &action, NULL);
}


/**
 * init_ignore():
 * Initialize SIG_IGN for signal @signum.
 * @signum: number of the signal
 *
 * Returns:
 * -1 if an error occurred
 */
int init_ignore(int signum) {
    struct sigaction action  = {0};
    action.sa_handler = SIG_IGN;

    sigemptyset(&action.sa_mask);

    /* Restart any instructions interrupted by this signal */
    action.sa_flags = SA_RESTART;
    return sigaction(signum, &action, NULL);
}


/**
 * init_parent_sig_handlers():
 * Initializes the signal handlers for the parent process.
 */
void init_parent_sig_handlers() {
    exit_if_error(init_ignore(SIGINT)==-1, "sigaction");
    exit_if_error(init_handle_sigtstp(SIGTSTP, handle_sigtstp)==-1,
                  "sigaction");
}


/**
 * init_fg_child_sig_handlers():
 * Initializes the signal handlers for a fg child process.
 */
void init_fg_child_sig_handlers() {
    exit_if_error(init_handle_sigint(SIGINT, SIG_DFL)==-1, "sigaction");
    exit_if_error(init_ignore(SIGTSTP)==-1, "sigaction");
}


/**
 * init_bg_child_sig_handlers():
 * Initializes the signal handlers for a bg child process.
 */
void init_bg_child_sig_handlers() {
    exit_if_error(init_ignore(SIGINT)==-1, "sigaction");
    exit_if_error(init_ignore(SIGTSTP)==-1, "sigaction");
}

