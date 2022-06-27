#ifndef SMALLSH_H
#define SMALLSH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <sys/time.h>


// Values specified in assignment
#define CMD_PROMPT          ':'
#define PID_VAR             "$$"
#define INPUT_REDIR         '<'
#define OUTPUT_REDIR        '>'
#define BACKGROUND          '&'
#define COMMENT_CHAR        '#'
#define MAX_CHARS           2048
#define MAX_ARGS            512
#define SUCCESS              0
#define FAILURE              1

/*
 * Max size of array that stores bg processes
 * Value was chosen arbitrarily to align with the
 * small size of MAX_CHARS and MAX_ARGS. The actual number
 * of processes that the program can handle is untested.
 */
#define MAX_PROCS           1024

// permissions for files used redirection
#define S_IRW               0666

// default redirection for bg processes
#define BG_DEFAULT          "/dev/null"


// struct for storing parts of a command
struct cmd_line {
    char *cmd;
    char *args[MAX_ARGS];
    int argsc;
    char *input_file;
    char *output_file;
    bool background;
};


// built_in.c
ssize_t write_number(long num);
int mycd(char *args[], int argsc);
int get_status(int wstatus, int *type);
void report_status(int status, int type);
void myexit(struct cmd_line *cmd_parts, int status, pid_t *pids);
void free_cmd(struct cmd_line *cmd_parts);
bool is_built_in(char *cmd);
int run_built_in(struct cmd_line *cmd_parts, int status, int status_type, pid_t *pids);


// error_warn.c
void exit_if_error(bool condition, char *msg);
void bg_exit_if_error(bool condition);
bool warn_error(bool condition, char *msg);
bool warn_dne(bool condition, char *program, char *file_name);
bool warn_args(bool condition, int max_args);
bool warn_chars(bool condition, int max_chars);


// expand.c
char *find_replace(char *pattern, char *str, char *repl, int *nrepls);
int ltoa_buf(long num, char *buf, int size, int base);
int ltoa_dec_buf(long num, char *buf, int size);
char *expand_vars(char *str, char *pid, int *nrepls);
char *expand_vars_num(char *str, pid_t pid, int *nrepls);


// fg_only_mode.c
bool get_fg_only();
void toggle_fg_only();


// process_args.c
char *print_string_safe(char *str);
void print_cmd(struct cmd_line *cmd_parts);
bool valid_line(char *line, ssize_t nread);
void init_cmd_struct(struct cmd_line *cmd_parts);
struct cmd_line *get_cmd(bool *skip, char *pid);


// redirection.c
int redirect(char *file_name, char *msg, int flags, int new_fd);
int redirect_input(char *file_name);
int redirect_output(char *file_name);


// run.c
void init_procs(pid_t *procs);
int set_proc(pid_t *procs, pid_t pid);
int check_procs(pid_t *procs);
void clean_up_procs(pid_t *procs);
int execute_external(struct cmd_line *cmd_parts, char *input_file, char *output_file);
pid_t run_external_fg(struct cmd_line *cmd_parts, int *status, int *status_type);
void run_external_bg(struct cmd_line *cmd_parts, char *input_file, char *output_file, pid_t *procs);


// sig_handlers.c
void handle_sigtstp(int signum);
int init_handle_sigint(int signum, void (*handler)(int));
int init_handle_sigtstp(int signum, void (*handler)(int));
int init_ignore(int signum);
void init_parent_sig_handlers();
void init_fg_child_sig_handlers();
void init_bg_child_sig_handlers();

#endif // SMALLSH_H

