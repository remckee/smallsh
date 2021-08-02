/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
**********************/

#include "smallsh.h"

int main (int argc, char *argv[]) {
    int status_val = SUCCESS;
    int status_type = CLD_EXITED;
    const pid_t SH_PID = getpid();
    pid_t bg_procs[MAX_PROCS];

    init_parent_sig_handlers();
    init_procs(bg_procs);

    while (1) {
        bool skip;

        // Get command line from user.
        // get_cmd sets skip to false if command was successfully parsed
        // into cmd_parts struct and true otherwise
        struct cmd_line *cmd_parts = get_cmd(&skip, SH_PID);

        // If a valid non-comment command was successfully parsed,
        // route command to built-in or external.
        if (!skip) {
            assert(cmd_parts->cmd);
            if (is_built_in(cmd_parts->cmd)) {
                run_built_in(cmd_parts, status_val, status_type, bg_procs);

            } else {
                if (get_fg_only() || !(cmd_parts->background)) {
                    run_external_fg(cmd_parts, &status_val, &status_type);

                } else {
                    run_external_bg(cmd_parts, BG_DEFAULT, BG_DEFAULT, bg_procs);
                }
            }
        }

        // free memory that may have been allocated when parsing command
        free_cmd(cmd_parts);
    }

    return 0;
}
