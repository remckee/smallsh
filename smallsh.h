/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 2
Last edited: 07/26/2021
**********************/

#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#include <dirent.h>
#include <string.h>
//#include <argp.h>
//#include <fcntl.h>
//#include <sys/stat.h>
#include <assert.h>
#include <math.h>
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
#define MAX_CHARS           2048 //Does it include newline?
#define MAX_ARGS            512 //Does it include all args? Or just [arg1 arg2 ...]?
                                //
// https://man7.org/linux/man-pages/man3/regexec.3.html#EXAMPLES
// Calculates the number of elements in arr
#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

/* struct for storing parts of a command */
struct cmd_line {
    char *cmd;
    char *args[MAX_ARGS];
    int argsc;
    char *input_file;
    char *output_file;
    bool background;
};


/* functions.c */
void *malloc_safe(void *ptr, size_t size);
void free_safe(void *ptr);
char *print_string_safe(char *str);
bool warn_args(bool condition, int max_args);
bool warn_chars(bool condition, int max_chars);
int ltoa_buf(long num, char *buf, int size, int base);
int ltoa_dec_buf(long num, char *buf, int size);
//long find_replace(char *replace);
long find_replace(const char *const pattern, const char *const str, char *replace);
long find_replace_str(char *pattern, char *str, char *replace);

/* process_args.c */
int print_cmd(struct cmd_line *cmd_parts);
struct cmd_line *get_cmd();

/* exit.c */
/* cd.c */
int mycd(int argc, char *argv[]);


/* status.c */

#endif // SMALLSH_H

