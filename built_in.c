/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
**********************/

#include "smallsh.h"


// Changes current working directory to a named directory or HOME
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
    write_number(status);
    write(STDOUT_FILENO, &nl, 1);
}


void myexit(struct cmd_line *cmd_parts, int status) {
    // kill any other processes or jobs smallsh has started
    // atexit() ?

    // free memory that may have been allocated when parsing command
    free_cmd(cmd_parts);

    exit(status);
}


void clean_up_processes(pid_t *pids) {
    int i = 0;
    while (i < MAX_PROCS && pids[i] >= 0) {
        if (pids[i] > 0) {
            kill(pids[i], SIGTERM);
        }
        i++;
    }
}


bool is_built_in(char *cmd) {
    return !strcmp(cmd, "cd") || !strcmp(cmd, "status") || !strcmp(cmd, "exit");
}


int run_built_in(struct cmd_line *cmd_parts, int status, int status_type, pid_t *pids) {
    int result = -1;

    if (!strcmp(cmd_parts->cmd, "cd")) {
        result = mycd(cmd_parts->args, cmd_parts->argsc);
        warn_dne((result==-1) && (cmd_parts->argsc > 1), cmd_parts->cmd, cmd_parts->args[1]);

        char *cur_dir = NULL;
        cur_dir = getcwd(cur_dir, MAX_CHARS);
        free_safe (cur_dir);

    } else if (!strcmp(cmd_parts->cmd, "status")) {
        report_status(status, status_type);

    } else if (!strcmp(cmd_parts->cmd, "exit")) {
        clean_up_processes(pids);
        myexit(cmd_parts, status);
    } else {
        assert(!is_built_in(cmd_parts->cmd));
    }
    return result;
}
