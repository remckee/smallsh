/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
**********************/

#include "smallsh.h"

int main (int argc, char *argv[]) {
    /* if (argc != 2) { */
    /*     fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    int status_val = SUCCESS;
    char status_type = EXIT;
    bool fg_only = false;
    const pid_t SH_PID = getpid();
    pid_t bg_procs[MAX_PROCS];

    //static volatile int live_children = 0;
    init_parent_sig_handlers();
    init_procs(bg_procs);


    while (1) {
        bool skip;

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
                printf("built in\n");
                fflush(stdout);
                int result = run_built_in(cmd_parts, status_val, status_type);
                if (result==0) {
                    printf("success\n");
                    fflush(stdout);
                }
            } else {
                printf("external\n");
                fflush(stdout);

                if (fg_only) {
                    //
                }

                if (fg_only || !(cmd_parts->background)) {
                    printf("running in foreground\n");
                    fflush(stdout);
                    run_external_fg(cmd_parts, &status_val, &status_type, SH_PID);

                } else {
                    printf("running in background\n");
                    fflush(stdout);
                    run_external_bg(cmd_parts, BG_DEFAULT, BG_DEFAULT, SH_PID, bg_procs);

                    sigset_t empty_mask;
                    sigemptyset(&empty_mask);

                    if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
                        perror("sigsuspend");


                }
            }
        }
        // free memory that may have been allocated when parsing command
        free_cmd(cmd_parts);
    }

    return 0;
}
