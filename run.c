#include "smallsh.h"


/*
 * Initialize all elements in procs array to -1. The value -1 is used
 * to indicate which elements in the array haven't been used yet. Then,
 * when iterating through the array, we can stop when we reach an element
 * whose value is -1, rather than going through the entire array.
 */
void init_procs(pid_t *procs) {
    for (int i = 0; i < MAX_PROCS; i++) {
        procs[i] = -1;
    }
}

/*
 * Find the first element of procs array (which stores the pids of active
 * bg processes) whose value is negative or 0, and set it equal to pid.
 * An element will have a value of 0 if it was previously used by a bg
 * process that has since completed. An element will have a value of -1
 * if it has not been used yet. Returns the index where pid was inserted.
 */
int set_proc(pid_t *procs, pid_t pid) {
    int i = 0;
    while (i < MAX_PROCS && procs[i] > 0) {
        i++;
    }

    if (i < MAX_PROCS && procs[i] <= 0) {
        procs[i] = pid;
    }
    assert(i < MAX_PROCS);
    return i;
}


/*
 * Check on the bg processes in procs array, and report the status for any that
 * have terminated. Returns the number of processes that have terminated.
 */
int check_procs(pid_t *procs) {
    int pid;
    int i = 0;
    int wstatus;
    int num_term = 0;

    /*
     * Note that we only need to iterate through the portion of the array
     * where elements are non-negative, since elements are always added to
     * the first available slot, and elements with a value of -1 are unused.
     */
    while (i < MAX_PROCS && procs[i] >= 0) {
        /*
         * If procs[i] == 0, it means that the slot was used
         * for a pid that was found in a previous check
         */
        if (procs[i] > 0) {
            pid = waitpid(procs[i], &wstatus, WNOHANG);

            // report child pid and status if given process has finished
            if (pid == procs[i]) {
                num_term++;
                procs[i] = 0;

                printf("background pid %d is done: ", pid);
                fflush(stdout);

                int status, status_type;
                status = get_status(wstatus, &status_type);
                report_status(status, status_type);
            }
        }
        i++;
    }
    return num_term;
}


/*
 * Kill all active background processes using the given array of pids of
 * background processes.
 */
void clean_up_procs(pid_t *procs) {
    int i = 0;

    /*
     * Note that we only need to iterate through the portion of the array
     * where elements are non-negative, since elements are always added to
     * the first available slot, and elements with a value of -1 are unused.
     */
    while (i < MAX_PROCS && procs[i] >= 0) {
        if (procs[i] > 0) {
            kill(procs[i], SIGTERM);
        }
        i++;
    }
}


/*
 * Execute the command parsed into cmd_line struct. Returns -1 upon error,
 * but it will not return at all if it succeeds since it uses execvp. If
 * cmd_parts array doesn't indicate any file for input and/or output
 * redirection, the given arguments input_file and/or output_file will be used.
 */
int execute_external(struct cmd_line *cmd_parts, char *input_file, char *output_file) {
    int success = 0;

    /*
     * Set files used for input and output redirection. Use the file names
     * stored in cmd_parts if they are not NULL, otherwise use the file names
     * given in the other arguments to this function. If both are NULL, the
     * redirection will not occur.
     */
    char *input_red = (cmd_parts->input_file) ? (cmd_parts->input_file) : input_file;
    char *output_red = (cmd_parts->output_file) ? (cmd_parts->output_file) : output_file;

    // Input redirection: only occurs if file name in input_red is not NULL.
    if (input_red) {
        success = redirect_input(input_red);
    }

    // Output redirection: only occurs if file name in output_red is not NULL.
    if (output_red && success != -1) {
        success = redirect_output(output_red);
    }

    // If redirection did not return an error, execute command.
    if (success != -1) {
        success = execvp(cmd_parts->cmd, cmd_parts->args);
    }
    return success;
}


/*
 * Run external command in the foreground by forking a child.
 * Returns the pid of the child process.
 * Returns -1 if an error occurred.
 */
pid_t run_external_fg(struct cmd_line *cmd_parts, int *status, int *status_type) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        // fork error
        char *msg = "fork() failed!";
        exit_if_error(child_pid==-1, msg);

    } else if(child_pid == 0){
        /*
         * child process
         * Initialize fg child signal handlers, then execute command.
         */
        init_fg_child_sig_handlers();
        execute_external(cmd_parts, NULL, NULL);

        /*
         * Report an error and terminate child process if execute_external
         * returns, which would only occur if there was an error.
         */
        exit_if_error(1, cmd_parts->cmd);

    } else {
        /*
         * parent process
         * Errors in this portion of the function are fatal because they likely
         * indicate a problem in the code rather than invalid user input.
         */
        int wstatus;                    // status returned from waitpid
        sigset_t block_mask;            // used to block the SIGTSTP signal
                                        // while fg command is running

        // create a sigset_t mask containing only the SIGTSTP signal
        sigemptyset(&block_mask);
        sigaddset(&block_mask, SIGTSTP);

        /*
         * Block the SIGTSTP signal while the fg command is running so that
         * it can still complete in the fg if that signal is received.
         */
        int result = sigprocmask(SIG_SETMASK, &block_mask, NULL);
        exit_if_error((result == -1), "sigprocmask");

        // Wait for the fg command to complete
        pid_t temp_pid = waitpid(child_pid, &wstatus, 0);
        exit_if_error((temp_pid == -1), "waitpid");

        /*
         * release the block on SIGTSTP by calling sigprocmask with an empty
         * sigset_t
         */
        sigemptyset(&block_mask);
        result = sigprocmask(SIG_SETMASK, &block_mask, NULL);
        exit_if_error((result == -1), "sigprocmask");

        /*
         * Update status value and type.
         * Only report it if the process was killed by a signal.
         */
        *status = get_status(wstatus, status_type);
        if (*status_type == CLD_KILLED) {
            report_status(*status, *status_type);
        }
    }
    return child_pid;
}


// Run external command in the background by forking a child.
void run_external_bg(struct cmd_line *cmd_parts, char *input_file, char *output_file, pid_t *procs) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        // fork error
        perror("fork() failed!");
        fflush(stdout);
        bg_exit_if_error(child_pid==-1);

    } else if(child_pid == 0){
        /*
         * child process
         * Initialize bg child signal handlers, then execute command.
         */
        init_bg_child_sig_handlers();

        /*
         * Terminate child process if execute_external returns,
         * which would only occur if there was an error.
         */
        execute_external(cmd_parts, input_file, output_file);
        exit(FAILURE);

    } else {
        /*
         * parent process
         * report child pid when background process starts
         */
        printf("background pid is %d\n", child_pid);
        fflush(stdout);

        // Record the child pid in procs array in first available slot.
        set_proc(procs, child_pid);
    }
}

