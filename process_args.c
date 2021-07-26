/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/26/2021
**********************/

#include "smallsh.h"


//int main(int argc, char *argv[]){
int print_cmd(struct cmd_line *cmd_parts) {
    int result = 0;
/* struct cmd_line { */
/*     char *cmd; */
/*     int argsc; */
/*     char *input_file; */
/*     char *output_file; */
/*     bool background; */
/*     char **newargv; */
/* }; */

    if (cmd_parts->cmd) {
        printf("command from main: %s\n", cmd_parts->cmd);
        fflush(NULL);
    } else {
        // error, since the command part is required
    }

    if (cmd_parts->input_file) {
        printf("input file from main: %s\n", cmd_parts->input_file);
        fflush(NULL);
    } else {
        printf("no input file\n");
        fflush(NULL);
    }

    if (cmd_parts->output_file) {
        printf("output file from main: %s\n", cmd_parts->output_file);
        fflush(NULL);
    } else {
        printf("no output file\n");
        fflush(NULL);
    }

    if (cmd_parts->background) {
        printf("process in the background\n");
        fflush(NULL);
    } else {
        printf("process in the foreground\n");
        fflush(NULL);
    }

    for (int i = 0; i < cmd_parts->argsc && cmd_parts->args[i]; i++) {
        if (cmd_parts->args[i]) {
            printf("%d | %s\n", i, cmd_parts->args[i]);
        } else {
            printf("%d | NULL\n", i);
        }

    }
    //exit(EXIT_SUCCESS);
    return result;
}

/* struct cmd_line *get_cmd() { */
/*     printf("%c ", CMD_PROMPT); */
/*     fflush(NULL); */
/* } */
