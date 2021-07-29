/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
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
        myexit(cmd_parts, status);
    } else {
        assert(!is_built_in(cmd_parts->cmd));
    }
    return result;
}


int execute_external(struct cmd_line *cmd_parts, char *input_file, char *output_file) {
    int success = 0;
    char *input_red = (cmd_parts->input_file) ? (cmd_parts->input_file) : input_file;
    char *output_red = (cmd_parts->output_file) ? (cmd_parts->output_file) : output_file;

    // input redirection
    if (input_red) {
        success = redirect_input(input_red);
    }

    // output redirection
    if (output_red && success != -1) {
        success = redirect_output(output_red);
    }

    // execute command
    if (success != -1) {
        success = execvp(cmd_parts->cmd, cmd_parts->args);
    }
    return success;
}


// run external command in the foreground
void run_external_fg(struct cmd_line *cmd_parts, int *status, char *status_type) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        // fork error
        char *msg = "fork() failed!";
        fg_exit_if_error(cmd_parts, child_pid==-1, msg, status, status_type);

    } else if(child_pid == 0){
        // child process
        execute_external(cmd_parts, NULL, NULL);
        fg_exit_if_error(cmd_parts, 1, cmd_parts->cmd, status, status_type);

    } else{
        // parent process
        child_pid = waitpid(child_pid, status, 0); //WNOHANG
        *status = get_status(*status, status_type);
    }
}


// run external command in the background
void run_external_bg(struct cmd_line *cmd_parts, char *input_file, char *output_file) {
    pid_t child_pid = fork();
    int status;
    char status_type;

    if(child_pid == -1){
        // fork error
        perror("fork() failed!");
        fflush(stdout);
        bg_exit_if_error(cmd_parts, child_pid==-1);

    } else if(child_pid == 0){
        // child process
        execute_external(cmd_parts, input_file, output_file);
        bg_exit_if_error(cmd_parts, 1);

    } else {
        // parent process
        // report child pid when background process starts
        printf("background pid is %d\n", child_pid);
        fflush(stdout);

        child_pid = waitpid(child_pid, &status, WNOHANG);

        // report child pid when background process finishes
        printf("background pid %d is done: ", child_pid);
        fflush(stdout);

        status = get_status(status, &status_type);
        report_status(status, status_type);
    }
}

