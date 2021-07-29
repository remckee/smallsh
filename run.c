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

// run external command. File pointer *parent implements behavior that differs
// between foreground and background modes.
void run_external(struct cmd_line *cmd_parts, int *status, char *status_type,
                  char *input_file, char *output_file,
                  pid_t (*parent) (pid_t *child_pid, int *child_status)) {

    int child_status;
    pid_t child_pid = fork();

    if(child_pid == -1){
        // fork error
        perror("fork() failed!");
        fflush(stdout);
        *status_type = EXIT;
        *status = FAILURE;
        exit(*status);

    } else if(child_pid == 0){
        // child process
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
        //child_pid = waitpid(child_pid, &child_status, 0); //WNOHANG
        child_pid = parent(&child_pid, &child_status);
        *status = get_status(child_status, status_type);
        //printf("parent reports status: ");
        //fflush(stdout);
        report_status(*status, *status_type);
    }
}


void run_external_fg(struct cmd_line *cmd_parts, int *status, char *status_type) {

    pid_t (*parent) (pid_t*, int*) = run_external_fg_parent;
    run_external(cmd_parts, status, status_type, NULL, NULL, parent);
}


pid_t run_external_fg_parent(pid_t *child_pid, int *child_status) {
    *child_pid = waitpid(*child_pid, child_status, 0);

    return *child_pid;
}


void run_external_bg(struct cmd_line *cmd_parts, int *status, char *status_type) {
    //run_external(cmd_parts, status, status_type, BG_DEFAULT, BG_DEFAULT);

    pid_t (*parent) (pid_t*, int*) = run_external_bg_parent;
    run_external(cmd_parts, status, status_type,  BG_DEFAULT, BG_DEFAULT, parent);
}


pid_t run_external_bg_parent(pid_t *child_pid, int *child_status) {
    // report child pid when background process starts
    printf("background pid is %d\n", *child_pid);
    fflush(stdout);
    *child_pid = waitpid(*child_pid, child_status, WNOHANG);

    // report child pid when background process finishes
    printf("background pid %d is done: ", *child_pid);
    fflush(stdout);
    return *child_pid;
}


void run_external_fg_child() {
}


void run_external_bg_child() {
}


