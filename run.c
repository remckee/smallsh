/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


bool is_built_in(char *cmd) {
    return !strcmp(cmd, "cd") || !strcmp(cmd, "status") || !strcmp(cmd, "exit");
}


int run_built_in(struct cmd_line *cmd_parts, int status, char status_type) {
    int result = -1;

    if (!strcmp(cmd_parts->cmd, "cd")) {
        printf("run cd\n");
        fflush(stdout);
        result = mycd(cmd_parts->args, cmd_parts->argsc);
        warn_dne((result==-1) && (cmd_parts->argsc > 1), cmd_parts->cmd, cmd_parts->args[1]);

        char *cur_dir = NULL;
        cur_dir = getcwd(cur_dir, MAX_CHARS);
        printf("changed directory to %s ", cur_dir);
        fflush(stdout);
        free_safe (cur_dir);

    } else if (!strcmp(cmd_parts->cmd, "status")) {
        printf("run status\n");
        fflush(stdout);
        report_status(status, status_type);

    } else if (!strcmp(cmd_parts->cmd, "exit")) {
        printf("run exit\n");
        fflush(stdout);
        myexit();
    } else {
        assert(!is_built_in(cmd_parts->cmd));
    }
    return result;
}


void run_external(struct cmd_line *cmd_parts, int *status, char *status_type) {
    int child_status;
    pid_t child_pid = fork();

    if(child_pid == -1){
        perror("fork() failed!");
        *status_type = EXIT;
        *status = FAILURE;
        exit(*status);

    } else if(child_pid == 0){
        // child process
        int success = 0;

        // input redirection
        if (cmd_parts->input_file) {
            success = redirect_input(cmd_parts->input_file);
        }

        // output redirection
        if (cmd_parts->output_file && success != -1) {
            success = redirect_output(cmd_parts->output_file);
        }

        // execute command
        if (success != -1) {
            execvp(cmd_parts->cmd, cmd_parts->args);

            // if execvp returns, display error message,
            // update status, and terminate child process
            perror(cmd_parts->cmd);
        }

        *status_type = EXIT;
        *status = FAILURE;
        exit(*status);

    } else{
        // parent process
        child_pid = waitpid(child_pid, &child_status, WNOHANG);
        *status = get_status(child_status, status_type);
        printf("parent reports status: ");
        report_status(*status, *status_type);
    }
}


void run_background(pid_t child_pid) {
//"background pid is "





//"background pid %d is done: exit value 0"
}
