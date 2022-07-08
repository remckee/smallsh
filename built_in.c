#include "smallsh.h"


/**
 * write_number():
 * Write @num to stdout using only non-reentrant functions.
 * @num: the number to write
 *
 * Return:
 * a positive value on success or a negative value on error
 */
ssize_t write_number(long num) {
    int num_len = 21; /* LONG_MAX (2^63 - 1) has 19 chars + sign + \0 */
    char num_ascii[num_len];

    num_len = ltoa_dec_buf(num, num_ascii, num_len);

    if (num_len > 0) {
        num_len = write(STDOUT_FILENO, &num_ascii, num_len);
        fflush(stdout);
    }

    num_len = (num_len == 0) ? -1 : num_len;

    return num_len;
}


/**
 * mycd():
 * Change current working directory to a named directory (@args[1]) or HOME.
 * @args: an array of user-provided arguments
 * @argsc: the number of elements in args
 *
 * Return:
 * a positive value on success or a negative value on error
 */
int mycd(char *args[], int argsc) {
    int success = -1;

    if (argsc > 1) {
        success = chdir(args[1]);

    } else if (args[0]) {
        success = chdir(getenv("HOME"));
        assert(success != -1);
    }
    return success;
}


/**
 * get_status():
 * Determine whether the process exited or was terminated by a signal and get
 * the status value.
 * @wstatus: the status value obtained from a waitpid() call
 * @type: out parameter indicating whether the last foreground process exited
 *        (CLD_EXITED) or was terminated by a signal (CLD_KILLED)
 * Return:
 * either the exit value or the number of the signal that caused termination of
 * the last foreground process
 */
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


/**
 * report_status():
 * Write the @status and @type of the last foreground process to stdout.
 * @status: either exit value or number of the signal that caused termination
 * @type: the type of status, indicating whether the process exited (CLD_EXITED)
 *        or was terminated by a signal (CLD_KILLED)
 */
void report_status(int status, int type) {
    char *message;
    size_t msg_len = 0;
    char nl = '\n';

    if (type == CLD_KILLED) {
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


/**
 * @myexit():
 * Perform clean-up and then exit shell.
 * @cmd_parts: data allocated by last command
 * @status: exit value
 * @pids: an array of child processes to kill
 */
void myexit(struct cmd_line *cmd_parts, int status, pid_t *pids) {
    free_cmd(cmd_parts);
    clean_up_procs(pids);
    exit(status);
}


/**
 * free_cmd():
 * Free memory allocated when parsing the most recent command.
 * @cmd_parts: data allocated by last command
 */
void free_cmd(struct cmd_line *cmd_parts) {
    free(cmd_parts->cmd);
    free(cmd_parts->input_file);
    free(cmd_parts->output_file);

    /*
     * Start at 1 because cmd_parts->args[0] is a pointer to cmd_parts->cmd,
     * which was already freed.
     */
    for (int i = 1; i < cmd_parts->argsc && cmd_parts->args[i]; i++) {
        free(cmd_parts->args[i]);
    }

    free(cmd_parts);
}


/**
 * is_built_in():
 * Determine whether a command is one of the built in commands.
 * @cmd: the name of the command
 *
 * Return:
 * true if @cmd is a built in command, false otherwise
 */
bool is_built_in(char *cmd) {
    return !strcmp(cmd, "cd") || !strcmp(cmd, "status") || !strcmp(cmd, "exit");
}


/**
 * run_built_in():
 * Run one of the built in commands.
 * @cmd_parts: the parsed parts of the command
 * @status: either exit value or number of the signal that caused termination of
 *          the last foreground process
 * @status_type: the type of status, indicating whether the process exited
 *               (CLD_EXITED) or was terminated by a signal (CLD_KILLED)
 *
 * Return:
 * a positive value on success or a negative value on error
 */
int run_built_in(struct cmd_line *cmd_parts, int status, int status_type,
                 pid_t *pids) {
    int result = 0;

    if (!strcmp(cmd_parts->cmd, "cd")) {
        result = mycd(cmd_parts->args, cmd_parts->argsc);

        /* Display a warning if the given directory does not exist */
        warn_dne((result == -1) && (cmd_parts->argsc > 1), cmd_parts->cmd,
                 cmd_parts->args[1]);

    } else if (!strcmp(cmd_parts->cmd, "status")) {
        report_status(status, status_type);

    } else if (!strcmp(cmd_parts->cmd, "exit")) {
        myexit(cmd_parts, status, pids);

    } else {
        assert(!is_built_in(cmd_parts->cmd));
    }
    return result;
}
