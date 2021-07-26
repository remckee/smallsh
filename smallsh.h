/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 2
Last edited: 07/22/2021
**********************/

#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#include <dirent.h>
#include <string.h>
//#include <argp.h>
//#include <fcntl.h>
//#include <sys/stat.h>
//#include <assert.h>
//#include <math.h>
//#include <time.h>
#include <stdbool.h>
//#include <utime.h>
#include <errno.h>
//#include <elf.h>

#define CMD_PROMPT          ':'
#define PID_VAR             "$$"
#define INPUT_REDIR         '<'
#define OUTPUT_REDIR        '>'
#define BACKGROUND          '&'
#define COMMENT_CHAR        '#'
#define MAX_CMD_CHARS       2048 //Does it include newline?
#define MAX_CMD_ARGS        512 //Does it include all args? Or just [arg1 arg2 ...]?

/* struct for storing parts of a command */
struct cmd_line {
    char *cmd;
    int argsc;
    char *input_file;
    char *output_file;
    bool background;
    char **newargv;
};


/* process_args.c */
int process(struct cmd_line *cmd_parts);


/* exit.c */
/* cd.c */
int mycd(int argc, char *argv[]);


/* status.c */

#endif // SMALLSH_H

