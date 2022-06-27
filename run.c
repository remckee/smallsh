#include "smallsh.h"


/**
 * init_procs():
 * Initialize all elements in an array to -1. Then, when iterating through the
 * array, we can stop when we reach an element whose value is -1, rather than
 * going through the entire array.
 * @procs: the array to initialize
 */
void init_procs(pid_t *procs) {
    for (int i = 0; i < MAX_PROCS; i++) {
        procs[i] = -1;
    }
}

/**
 * set_proc():
 * Set the first available slot in @procs to @pid. An available slot has a
 * value <= 0. A slot will be 0 if it was used by a bg process that has
 * completed. A slot will be -1 if it has not been used yet.
 * @procs: the array of processes
 * @pid: the pid of the process being added
 *
 * Returns:
 * the index where @pid was inserted
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


/**
 * check_procs():
 * Check on the bg processes in @procs, and report the status for any that
 * have terminated.
 * @procs: the array of processes
 *
 * Returns:
 * the number of processes that have terminated
 */
int check_procs(pid_t *procs) {
    int pid;
    int i = 0;
    int wstatus;
    int num_term = 0;

    while (i < MAX_PROCS && procs[i] >= 0) {
        if (procs[i] > 0) {
            pid = waitpid(procs[i], &wstatus, WNOHANG);

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


/**
 * clean_up_procs():
 * Kill all active background processes in array of pids @procs.
 * @procs: the array of pids
 */
void clean_up_procs(pid_t *procs) {
    int i = 0;

    while (i < MAX_PROCS && procs[i] >= 0) {
        if (procs[i] > 0) {
            kill(procs[i], SIGTERM);
        }
        i++;
    }
}


/**
 * execute_external():
 * Execute the command parsed into @cmd_line struct.
 * @cmd_parts: the parts of the parsed command
 * @input_file: name of file for input redirection if none specified in
 *              @cmd_parts
 * @output_file: name of file for output redirection if none specified in
 *               @cmd_parts
 *
 * If @cmd_parts does not indicate any file for input and/or output redirection,
 * @input_file and/or @output_file will be used. If both @cmd_parts->input_file
 * and @input_file are NULL, input redirection will not occur. If both
 * @cmd_parts->output_file and @output_file are NULL, output redirection will
 * not occur.
 *
 * Returns:
 * -1 upon error, but will not return at all if it succeeds
 */
int execute_external(struct cmd_line *cmd_parts, char *input_file, char *output_file) {
    int success = 0;
    char *input_red = (cmd_parts->input_file) ? (cmd_parts->input_file) : input_file;
    char *output_red = (cmd_parts->output_file) ? (cmd_parts->output_file) : output_file;

    if (input_red) {
        success = redirect_input(input_red);
    }

    if (output_red && success != -1) {
        success = redirect_output(output_red);
    }

    if (success != -1) {
        success = execvp(cmd_parts->cmd, cmd_parts->args);
    }
    return success;
}


/**
 * run_external_fg():
 * Run external command parsed into @cmd_parts in the foreground by forking a
 * new process.
 * @cmd_parts: the parts of the parsed command
 * @status: a reference variable to return command status to
 * @status_type: a reference variable to return the type of the status to
 *
 * Returns:
 * the pid of the new process or -1 if an error occurred
 */
pid_t run_external_fg(struct cmd_line *cmd_parts, int *status, int *status_type) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        char *msg = "fork() failed!";
        exit_if_error(child_pid==-1, msg);

    } else if(child_pid == 0){
        init_fg_child_sig_handlers();
        execute_external(cmd_parts, NULL, NULL);

        /*
         * Report an error and terminate child process if execute_external()
         * returns, which would only occur if there was an error.
         */
        exit_if_error(1, cmd_parts->cmd);

    } else {
        int wstatus;                    /* status returned from waitpid */
        sigset_t block_mask;            /* used to block the SIGTSTP signal */
                                        /* while fg command is running */

        /* create a sigset_t mask containing only the SIGTSTP signal */
        sigemptyset(&block_mask);
        sigaddset(&block_mask, SIGTSTP);

        /*
         * Block the SIGTSTP signal while the fg command is running so that
         * it can still complete in the fg if that signal is received.
         */
        int result = sigprocmask(SIG_SETMASK, &block_mask, NULL);
        exit_if_error((result == -1), "sigprocmask");

        /* Wait for the fg command to complete */
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
         * Update status value and type, but only report it if the process
         * was killed by a signal.
         */
        *status = get_status(wstatus, status_type);
        if (*status_type == CLD_KILLED) {
            report_status(*status, *status_type);
        }
    }
    return child_pid;
}


/**
 * run_external_bg():
 * Run external command parsed into @cmd_parts in the background by forking a
 * new process.
 * @cmd_parts: the parts of the parsed command
 * @input_file: name of file for input redirection if none specified in
 *              @cmd_parts
 * @output_file: name of file for output redirection if none specified in
 *               @cmd_parts
 * @procs: an array in which to record the pid of the new process
 *
 * See execute_external() more info about @input_file and @output_file.
 */
void run_external_bg(struct cmd_line *cmd_parts, char *input_file, char *output_file, pid_t *procs) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        perror("fork() failed!");
        fflush(stdout);
        bg_exit_if_error(child_pid==-1);

    } else if(child_pid == 0){
        init_bg_child_sig_handlers();
        execute_external(cmd_parts, input_file, output_file);

        /*
         * Terminate child process if execute_external() returns,
         * which would only occur if there was an error.
         */
        exit(FAILURE);

    } else {
        printf("background pid is %d\n", child_pid);
        fflush(stdout);

        set_proc(procs, child_pid);
    }
}

