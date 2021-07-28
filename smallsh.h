/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 2
Last edited: 07/27/2021
**********************/

#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <regex.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#include <dirent.h>
#include <string.h>
//#include <argp.h>
#include <fcntl.h>
//#include <sys/stat.h>
#include <assert.h>
#include <math.h>
//#include <time.h>
#include <stdbool.h>
//#include <utime.h>
#include <errno.h>
#include <signal.h>
//#include <elf.h>


#define CMD_PROMPT          ':'
#define PID_VAR             "$$"
#define INPUT_REDIR         '<'
#define OUTPUT_REDIR        '>'
#define BACKGROUND          '&'
#define COMMENT_CHAR        '#'
#define MAX_CHARS           2048
#define MAX_ARGS            512

#define EXIT                'e'
#define TERM                't'
#define SUCCESS              0
#define FAILURE              1

#define S_IRW               0666


/* struct for storing parts of a command */
struct cmd_line {
    char *cmd;
    char *args[MAX_ARGS];
    int argsc;
    char *input_file;
    char *output_file;
    bool background;
};


/* cmd_line.c */
void init_cmd_struct(struct cmd_line *cmd_parts);


/* safe.c */
void *malloc_safe(void *ptr, size_t size);
void free_safe(void *ptr);
char *print_string_safe(char *str);


/* ltoa.c */
int ltoa_buf(long num, char *buf, int size, int base);
int ltoa_dec_buf(long num, char *buf, int size);


/* expand.c */
char *find_replace(char *pattern, char *str, char *repl, int *nrepls);
char *expand_vars(char *str, pid_t pid, int *nrepls);


/* process_args.c */
int print_cmd(struct cmd_line *cmd_parts);
bool valid_line(char *line, ssize_t nread);
struct cmd_line *get_cmd(bool *skip);


/* run.c */
bool is_built_in(char *cmd);
int run_built_in(struct cmd_line *cmd_parts, int status, char status_type);
void run_external(struct cmd_line *cmd_parts, int *status, char *status_type);


/* exit.c */
void myexit();


/* cd.c */
int mycd(char *args[], int argsc);


/* status.c */
int get_status(int wstatus, char *status_type);
void report_status(int status, char status_type);


/* redirection.c */
int redirect(char *file_name, char *msg, int flags, int new_fd);
int redirect_input(char *file_name);
int redirect_output(char *file_name);


/* error_warn.c */
void exit_if_error(bool condition, char *file_name, char *function);
void warn_dne(bool condition, char *program, char *file_name);
bool warn_args(bool condition, int max_args);
bool warn_chars(bool condition, int max_chars);


#endif // SMALLSH_H

