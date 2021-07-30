/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


int print_cmd(struct cmd_line *cmd_parts) {
    int result = 0;
    if (cmd_parts->cmd) {
        printf("command: %s ", print_string_safe(cmd_parts->cmd));
        fflush(stdout);
    } else {
        assert(cmd_parts->cmd);
    }

    if (cmd_parts->input_file) {
        printf("input file: %s ", print_string_safe(cmd_parts->input_file));
        fflush(stdout);
    } else {
        printf("no input file ");
        fflush(stdout);
    }

    if (cmd_parts->output_file) {
        printf("output file: %s ", print_string_safe(cmd_parts->output_file));
        fflush(stdout);
    } else {
        printf("no output file ");
        fflush(stdout);
    }

    if (cmd_parts->background) {
        printf("background\n");
        fflush(stdout);
    } else {
        printf("foreground\n");
        fflush(stdout);
    }

    for (int i = 0; i < cmd_parts->argsc && cmd_parts->args[i]; i++) {
        if (cmd_parts->args[i]) {
            printf("%d | %s\n", i, print_string_safe(cmd_parts->args[i]));
            fflush(stdout);
        } else {
            printf("%d | NULL\n", i);
            fflush(stdout);
        }

    }
    return result;
}


bool valid_line(char *line, ssize_t nread) {
    bool skip = false;

    // Clear any errors from stdin.
    if (ferror(stdin)) {
        clearerr(stdin);
        skip = true;
    }

    // Check whether this command should be skipped because the read was invalid
    // or only contained a newline char, it was a comment, or it exceeded the char limit.
    skip = (nread <= 1) || (line[0]==COMMENT_CHAR) || warn_chars(nread > MAX_CHARS+1, MAX_CHARS);

    // If the command line exceeds the character limit, warn_chars will display a
    // warning message to the user and return true.
    return skip;
}


struct cmd_line *get_cmd(bool *skip, pid_t pid) {
    printf("%c ", CMD_PROMPT);
    fflush(stdout);

    char *line = NULL;          // Stores entire user input, which will be copied
                                // to buf if it does not exceed the char limit.
    char buf[MAX_CHARS+2];      // MAX_CHARS+2 allows room for \n and \0 at the end
    memset(buf, '\0', sizeof(buf));
    ssize_t nread = 0;
    size_t len = 0;

    nread = getline(&line, &len, stdin);
    *skip = valid_line(line, nread);

    struct cmd_line *cmd_parts = NULL;
    cmd_parts = malloc(sizeof(struct cmd_line));
    init_cmd_struct(cmd_parts);

    if (!(*skip)) {
        memset(buf, '\0', sizeof(buf));
        strcpy(buf, line);
        char *arg = NULL;
        char *save_ptr;

        // try to parse first argument (command)
        arg = strtok_r(buf, " \n", &save_ptr);

        // Skip rest of command if arg is NULL (possibly due to containing only whitespace)
        // or command is a comment line not caught previously.
        // Note that a comment line with leading whitespace, if it exceeds the char limit,
        // should be caught by warn_chars, not here, to avoid buffer overflow.
        *skip = !arg || (arg[0]==COMMENT_CHAR);

        if (!(*skip)) {
            int exp_count = 0;
            arg = expand_vars(arg, pid, &exp_count);

            if (arg) {
                cmd_parts->cmd = strdup(arg);
                cmd_parts->args[cmd_parts->argsc] = cmd_parts->cmd;
                (cmd_parts->argsc)++;
                if (exp_count > 0) {
                    free_safe(arg);
                }
            } else {
                *skip = true;
            }

            int input_next = -1;        // whether the next arg should be an input file
            int output_next = -1;       // whether the next arg should be an output file
            while (arg && !(*skip)) {
                arg = strtok_r(NULL, " \n", &save_ptr);
                if (arg) {
                    cmd_parts->background = false;
                    long arg_len = strlen(arg);
                    exp_count = 0;
                    arg = expand_vars(arg, pid, &exp_count);

                    if (!arg) {
                        *skip = true;
                    } else if (input_next==1) {
                        cmd_parts->input_file = strdup(arg);
                        input_next = 0;
                    } else if (output_next==1) {
                        cmd_parts->output_file = strdup(arg);
                        output_next = 0;

                    // input_next *= -1 will change the value:
                    // -1 -> 1          0 -> 0          1 -> -1
                    // such that, if initialized to -1 and set to 0 after
                    // using, it can only be used once.
                    // Works similarly for output_next.
                    } else if (arg_len==1 && arg[0]==INPUT_REDIR) {
                        input_next *= -1;

                    } else if (arg_len==1 && arg[0]==OUTPUT_REDIR) {
                        output_next *= -1;

                    } else if (arg_len==1 && arg[0]==BACKGROUND) {
                        // if there are any arguments after this,
                        // cmd_parts->background will be changed to false
                        // near beginning of next iteration of while loop
                        cmd_parts->background = true;

                    } else {
                        // If the command line args will exceed the argument limit
                        // after incrementing, warn_args will display a warning message
                        // to the user and return true.
                        (*skip) = warn_args((cmd_parts->argsc)+1 > MAX_ARGS, MAX_ARGS);

                        if (!(*skip)) {
                            cmd_parts->args[cmd_parts->argsc] = strdup(arg);
                            (cmd_parts->argsc)++;
                        }
                    }
                    if (exp_count > 0) {
                        free_safe(arg);
                    }
                }
            }
            //if (!(*skip)) {
            //    print_cmd(cmd_parts);
            //}
        }
    }

    free_safe(line);
    line = NULL;
    len = 0;
    return cmd_parts;
}

