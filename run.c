/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
**********************/

#include "smallsh.h"

void init_procs(pid_t *procs) {
    for (int i = 0; i < MAX_PROCS; i++) {
        procs[i] = -1;
    }
}

int set_proc(pid_t *procs, pid_t pid) {
    int i = 0;
    while (i < MAX_PROCS && procs[i] > 0) {
        i++;
    }

    if (procs[i] <= 0) {
        procs[i] = pid;
    }
    assert(i < MAX_PROCS);
    return i;
}


int execute_external(struct cmd_line *cmd_parts, char *input_file, char *output_file) {
    int success = 0;
    char *input_red = (cmd_parts->input_file) ? (cmd_parts->input_file) : input_file;
    char *output_red = (cmd_parts->output_file) ? (cmd_parts->output_file) : output_file;

    // input redirection
    if (input_red) {
        success = redirect_input(cmd_parts, input_red);
    }

    // output redirection
    if (output_red && success != -1) {
        success = redirect_output(cmd_parts, output_red);
    }

    // execute command
    if (success != -1) {
        success = execvp(cmd_parts->cmd, cmd_parts->args);
    }
    return success;
}


// run external command in the foreground
pid_t run_external_fg(struct cmd_line *cmd_parts, int *status, int *status_type) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        // fork error
        char *msg = "fork() failed!";
        fg_exit_if_error(cmd_parts, child_pid==-1, msg, status, status_type);

    } else if(child_pid == 0){
        // child process
        init_fg_child_sig_handlers();
        execute_external(cmd_parts, NULL, NULL);
        fg_exit_if_error(cmd_parts, 1, cmd_parts->cmd, status, status_type);

    } else {
        // parent process
        siginfo_t info;
        memset(&info, 0, sizeof(siginfo_t));
        sigset_t block_mask;

        sigemptyset(&block_mask);
        sigaddset(&block_mask, SIGTSTP);

        int result = sigprocmask(SIG_SETMASK, &block_mask, NULL);
        if (result == -1) {
            perror("sigprocmask");
        }

        child_pid = waitid(P_PID, child_pid, &info, WEXITED);

        sigemptyset(&block_mask);
        result = sigprocmask(SIG_SETMASK, &block_mask, NULL);
        if (result == -1) {
            perror("sigprocmask");
        }

        *status = info.si_status;
        *status_type = info.si_code;
        if (*status_type == CLD_KILLED) {
            report_status(*status, *status_type);
        }
    }
    return child_pid;
}


// run external command in the background
void run_external_bg(struct cmd_line *cmd_parts, char *input_file, char *output_file, pid_t *procs) {
    pid_t child_pid = fork();

    if(child_pid == -1){
        // fork error
        perror("fork() failed!");
        fflush(stdout);
        bg_exit_if_error(cmd_parts, child_pid==-1);

    } else if(child_pid == 0){
        // child process
        init_bg_child_sig_handlers();
        execute_external(cmd_parts, input_file, output_file);
        bg_exit_if_error(cmd_parts, 1);

    } else {
        // parent process
        // report child pid when background process starts
        printf("background pid is %d\n", child_pid);
        fflush(stdout);

        // record the child pid in procs array in first available slot
        set_proc(procs, child_pid);
    }
}

