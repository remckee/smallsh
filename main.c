/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/27/2021
**********************/

#include "smallsh.h"

int main (int argc, char *argv[]) {
    /* if (argc != 2) { */
    /*     fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    char quit = ' ';

    while (quit !='q') {
        bool skip;

        // Get command line from user.
        // get_cmd sets skip to false if command was successfully parsed
        // into cmd_parts struct and true otherwise
        struct cmd_line *cmd_parts = get_cmd(&quit, &skip);

        // If a valid non-comment command was successfully parsed,
        // route command to built-in or external.
        if (!skip) {
            assert(cmd_parts->cmd);
            if (is_built_in(cmd_parts->cmd)) {
                printf("built in\n");
                fflush(stdout);
                int result = run_built_in(cmd_parts);
                if (result==0) {
                    printf("success\n");
                    fflush(stdout);
                }
            } else {
                printf("external\n");
                fflush(stdout);
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

    //execlp()
    //perror("execv");   /* execve() returns only on error */
    //exit(EXIT_FAILURE);
    return 0;
}
