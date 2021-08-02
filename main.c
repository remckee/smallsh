/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
**********************/

#include "smallsh.h"

int main (int argc, char *argv[]) {
    /* if (argc != 2) { */
    /*     fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    int status_val = SUCCESS;
    int status_type = CLD_EXITED;
    //bool fg_only_loc = fg_only;
    const pid_t SH_PID = getpid();
    pid_t bg_procs[MAX_PROCS];
    //pid_t fg_proc = 0;

    //static volatile int live_children = 0;
    init_parent_sig_handlers();
    init_procs(bg_procs);


    while (1) {
        bool skip;

        //printf("top\n");
        //fflush(stdout);




        // check on background child processes
        //check_procs(bg_procs);

        // Get command line from user.
        // get_cmd sets skip to false if command was successfully parsed
        // into cmd_parts struct and true otherwise
        struct cmd_line *cmd_parts = get_cmd(&skip, SH_PID);

        // If a valid non-comment command was successfully parsed,
        // route command to built-in or external.
        if (!skip) {
            assert(cmd_parts->cmd);
            if (is_built_in(cmd_parts->cmd)) {
                //printf("built in\n");
                //fflush(stdout);
                int result = run_built_in(cmd_parts, status_val, status_type, bg_procs);
                if (result==0) {
                   // printf("success\n");
                    //fflush(stdout);
                }
            } else {
                //printf("external\n");
                //fflush(stdout);

                if (get_fg_only()) {
                    //
                }

                if (get_fg_only() || !(cmd_parts->background)) {
                    //printf("running in foreground\n");
                    //fflush(stdout);
                    run_external_fg(cmd_parts, &status_val, &status_type);
                    //printf("fg pid: %d\n", fg_proc);
                    fflush(stdout);

                } else {
                    //printf("running in background\n");
                    //fflush(stdout);
                    run_external_bg(cmd_parts, BG_DEFAULT, BG_DEFAULT, SH_PID, bg_procs);

                    /* sigset_t empty_mask; */
                    /* sigemptyset(&empty_mask); */

                    /* if (sigsuspend(&empty_mask) == -1 && errno != EINTR) { */
                    /*     perror("sigsuspend"); */
                    /*     fflush(stdout); */
                    /* } */

                    //alarm(3);
                    //for (;;)
                    //    pause();
                }
            }
        }

        /* sigset_t mask; */
        /* sigemptyset(&mask); */

        /* if (sigpending(&mask) == -1) { */
        /*     perror("sigpending"); */
        /*     fflush(stdout); */
        /* } else { // if (sigismember(&mask, SIGCHLD) == 1) */
        /*     //printf("check_procs\n"); */
        /*     //fflush(stdout); */
        /*     // check for terminated background child processes and report status */
        /*     check_procs(bg_procs); */


            /* int pid; */
            /* int status; */
            /* char status_type; */

            /* // check for terminated foreground child processes, and only report */
            /* // status if it was terminated by a signal */
            /* if((pid = waitpid(fg_proc, &status, WNOHANG)) > 0) { */
            /*     printf("getting status \n"); */
            /*     fflush(stdout); */
            /*     status = get_status(status, &status_type); */
            /*     if (status_type == CLD_KILLED) { */
            /*         report_status(status, status_type); */
            /*     } else { */
            /*         report_status(status, status_type); */
            /*     } */
            /* } */

            /* if (pid == -1) { */
            /*     perror("waitpid error\n"); */
            /*     fflush(stdout); */
            /* } */


        /* } */

        //printf("bottom\n");
        //fflush(stdout);

        // free memory that may have been allocated when parsing command
        free_cmd(cmd_parts);
    }

    return 0;
}
