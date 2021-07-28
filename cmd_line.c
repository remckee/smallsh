/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/27/2021
**********************/

#include "smallsh.h"


// Set the members of cmd_parts to initial values.
void init_cmd_struct(struct cmd_line *cmd_parts) {
    cmd_parts->cmd = NULL;
    cmd_parts->argsc = 0;
    cmd_parts->input_file = NULL;
    cmd_parts->output_file = NULL;
    cmd_parts->background = false;

    for (int i = 0; i < MAX_ARGS; i++) {
        cmd_parts->args[i] = NULL;
    }
}

