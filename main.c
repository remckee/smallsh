/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"

int main (int argc, char *argv[]) {
    /* if (argc != 2) { */
    /*     fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    int status_val = SUCCESS;
    char status_type = EXIT;

    while (1) {
        bool skip;

        // Get command line from user.
        // get_cmd sets skip to false if command was successfully parsed
        // into cmd_parts struct and true otherwise
        struct cmd_line *cmd_parts = get_cmd(&skip);

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
                run_external(cmd_parts, &status_val, &status_type);
            }
        }

        // free memory that may have been allocated to expand $$ variables
        free_safe(cmd_parts->cmd);
        free_safe(cmd_parts->input_file);
        free_safe(cmd_parts->output_file);

        // Start at 1 because cmd_parts->args[0] is a pointer to cmd_parts->cmd,
        // which was already freed.
        for (int i = 1; i < cmd_parts->argsc && cmd_parts->args[i]; i++) {
            free_safe(cmd_parts->args[i]);
        }

        // free memory allocated in get_cmd
        free_safe(cmd_parts);
    }
    return 0;
}
