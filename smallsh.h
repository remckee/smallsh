/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 08/02/2021
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
#include <time.h>
#include <stdbool.h>
//#include <utime.h>
#include <errno.h>
#include <signal.h>
//#include <elf.h>
#include <limits.h>
#include <sys/time.h>


#define CMD_PROMPT          ':'
#define PID_VAR             "$$"
#define INPUT_REDIR         '<'
#define OUTPUT_REDIR        '>'
#define BACKGROUND          '&'
#define COMMENT_CHAR        '#'
#define MAX_CHARS           2048
#define MAX_ARGS            512
#define MAX_PROCS           1024

#define EXIT                'e'
#define TERM                't'
#define SUCCESS              0
#define FAILURE              1

#define S_IRW               0666

#define BG_DEFAULT          "/dev/null"


/* struct for storing parts of a command */
struct cmd_line {
    char *cmd;
    char *args[MAX_ARGS];
    int argsc;
    char *input_file;
    char *output_file;
    bool background;
};


/* safe.c */
void *malloc_safe(void *ptr, size_t size);
void free_safe(void *ptr);
char *print_string_safe(char *str);
void free_cmd(struct cmd_line *cmd_parts);


/* expand.c */
char *find_replace(char *pattern, char *str, char *repl, int *nrepls);
char *expand_vars(char *str, pid_t pid, int *nrepls);
int ltoa_buf(long num, char *buf, int size, int base);
int ltoa_dec_buf(long num, char *buf, int size);


/* process_args.c */
int print_cmd(struct cmd_line *cmd_parts);
bool valid_line(char *line, ssize_t nread);
void init_cmd_struct(struct cmd_line *cmd_parts);
struct cmd_line *get_cmd(bool *skip, pid_t pid);


/* run.c */
void init_procs(pid_t *procs);
int set_proc(pid_t *procs, pid_t pid);
void check_procs(pid_t *procs);
bool is_built_in(char *cmd);
int run_built_in(struct cmd_line *cmd_parts, int status, int status_type, pid_t *pids);
int execute_external(struct cmd_line *cmd_parts, char *input_file, char *output_file);
pid_t run_external_fg(struct cmd_line *cmd_parts, int *status, int *status_type);
void run_external_bg(struct cmd_line *cmd_parts, char *input_file, char *output_file, const pid_t sh_pid, pid_t *procs);


/* sig_handlers.c */
ssize_t write_number(long num);
void handle_SIGINT(int signum);
void handle_SIGCHLD(int signum);
int init_handle_SIGCHLD();
int init_no_block(int signum, void (*handler)(int));
int init_ignore(int signum);
int init_pause_SIGTSTP();
int init_handle_SIGTSTP();

void init_parent_sig_handlers();
void init_fg_child_sig_handlers();
void init_bg_child_sig_handlers();


/* fg_only_mode.c */
bool get_fg_only();
void toggle_fg_only();


/* built_in.c */
int mycd(char *args[], int argsc);
int get_status(int wstatus, int *type);
void report_status(int status, int type);
void myexit(struct cmd_line *cmd_parts, int status);
void clean_up_processes(pid_t *pids);


/* redirection.c */
int redirect(struct cmd_line *cmd_parts, char *file_name, char *msg, int flags, int new_fd);
int redirect_input(struct cmd_line *cmd_parts, char *file_name);
int redirect_output(struct cmd_line *cmd_parts, char *file_name);


/* error_warn.c */
void fg_exit_if_error(struct cmd_line *cmd_parts, bool condition, char *msg,
                      int *status, int *status_type);
void bg_exit_if_error(struct cmd_line *cmd_parts, bool condition);
void warn_dne(bool condition, char *program, char *file_name);
bool warn_args(bool condition, int max_args);
bool warn_chars(bool condition, int max_chars);


#endif // SMALLSH_H

