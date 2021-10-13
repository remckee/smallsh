#include "smallsh.h"


// Writes num to stdout using only non-reentrant functions.
// returns a positive value on success
// returns a negative value on error
//
// Note that non-reentrancy is not required in the current version
// of the program, since none of the signal handlers use write_number(),
// but it was required for a signal handler that has been removed.
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


// Changes current working directory to a named directory or HOME.
// Returns -1 upon error.
int mycd(char *args[], int argsc) {
    int success = -1;

    if (argsc > 1) {
        // change to named directory if an argument was provided
        success = chdir(args[1]);

    } else if (args[0]) {
        // change to HOME directory
        success = chdir(getenv("HOME"));

        // assert if there is an error changing to HOME directory
        assert(success != -1);
    }
    return success;
}


// Uses the status value obtained from a waitpid call (wstatus) to determine
// whether the process exited or was terminated by a signal (type).
// Returns either the exit value or number of the signal that caused termination.
int get_status(int wstatus, int *type) {
    int status;

    if(WIFEXITED(wstatus)){
        *type = CLD_EXITED;
        status = WEXITSTATUS(wstatus);

    } else if(WIFSIGNALED(wstatus)) {
        *type = CLD_KILLED;
        status = WTERMSIG(wstatus);

    }
    return status;
}


// Write the status with the given type (exited vs. terminated by signal)
// and value to stdout.
//
// Note that non-reentrancy is not required in the current version
// of the program, since none of the signal handlers use report_status(),
// but it was required for a signal handler that has been removed.
void report_status(int status, int type) {
    char *message;
    size_t msg_len = 0;
    char nl = '\n';

    if (type==CLD_KILLED) {
        message = "terminated by signal ";
        msg_len = 22;
    } else {
        message = "exit value ";
        msg_len = 12;
    }

    write(STDOUT_FILENO, message, msg_len);
    fflush(stdout);
    write_number(status);
    write(STDOUT_FILENO, &nl, 1);
    fflush(stdout);
}


// Perform clean-up and then exit shell. Frees memory allocated by
// last command, kills all child processes, then exits with the given status.
void myexit(struct cmd_line *cmd_parts, int status, pid_t *pids) {
    free_cmd(cmd_parts);
    clean_up_procs(pids);
    exit(status);
}


// Free memory allocated when parsing most recent command.
void free_cmd(struct cmd_line *cmd_parts) {
    // free memory that may have been allocated to expand $$ variables
    free(cmd_parts->cmd);
    free(cmd_parts->input_file);
    free(cmd_parts->output_file);

    // Free memory allocated to elements of args array.
    // Start at 1 because cmd_parts->args[0] is a pointer to cmd_parts->cmd,
    // which was already freed.
    for (int i = 1; i < cmd_parts->argsc && cmd_parts->args[i]; i++) {
        free(cmd_parts->args[i]);
    }

    // free memory allocated to struct
    free(cmd_parts);
}


// Returns whether the given command is one of the three built in commands.
bool is_built_in(char *cmd) {
    return !strcmp(cmd, "cd") || !strcmp(cmd, "status") || !strcmp(cmd, "exit");
}


// Run one of the built in commands. Returns -1 upon error.
int run_built_in(struct cmd_line *cmd_parts, int status, int status_type, pid_t *pids) {
    int result = 0;

    if (!strcmp(cmd_parts->cmd, "cd")) {
        result = mycd(cmd_parts->args, cmd_parts->argsc);

        // Display a warning to the user if the given directory does not exist.
        warn_dne((result==-1) && (cmd_parts->argsc > 1), cmd_parts->cmd, cmd_parts->args[1]);

    } else if (!strcmp(cmd_parts->cmd, "status")) {
        report_status(status, status_type);

    } else if (!strcmp(cmd_parts->cmd, "exit")) {
        myexit(cmd_parts, status, pids);

    } else {
        assert(!is_built_in(cmd_parts->cmd));
    }
    return result;
}
