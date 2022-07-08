#include "smallsh.h"


/**
 * print_string_safe():
 * Ensure that @str is a non-empty string by changing it to "NULL" if NULL.
 * @str: the string to check
 *
 * Return:
 * @str if it is not NULL or the string "NULL" if @str is NULL
 */
char *print_string_safe(char *str) {
    return (str) ? str : "NULL";
}


/**
 * print_cmd():
 * Print the parsed parts of the command stored in @cmd_parts.
 * @cmd_parts: the parts of a parsed command
 */
void print_cmd(struct cmd_line *cmd_parts) {
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
}


/**
 * valid_line():
 * Determine whether a command @line should be skipped because the read was
 * invalid or only contained a newline char, it was a comment, or it exceeded
 * the char limit.
 * @line: the command line to check
 * @nread: the length of @line
 *
 * Return:
 * true if the command should be skipped, false otherwise
 */
bool valid_line(char *line, ssize_t nread) {
    bool skip = false;

    /* Clear any errors from stdin. */
    if (ferror(stdin)) {
        clearerr(stdin);
        skip = true;
    }

    skip = (nread <= 1) || (line[0] == COMMENT_CHAR)
            || warn_chars(nread > MAX_CHARS + 1, MAX_CHARS);

    return skip;
}


/**
 * init_cmd_struct():
 * Set the members of @cmd_parts to initial values.
 * @cmd_parts: a struct that will later hold the parts of a parsed command line
 */
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


/**
 * get_cmd():
 * Parse the inputted command line into a struct and return the struct.
 * @skip: out parameter that will be true if the command should not be processed
 *        and false otherwise
 * @pid: pid of the current process
 *
 * Return:
 * a struct pointer containing the parsed parts of the command line
 */
struct cmd_line *get_cmd(bool *skip, char *pid) {
    printf("%c ", CMD_PROMPT);
    fflush(stdout);

    char *line = NULL;      /* Stores entire user input, which will be copied */
                            /* to buf if it does not exceed the char limit. */
    char buf[MAX_CHARS + 2];/* MAX_CHARS + 2 allows room for \n and \0 */
    ssize_t nread = 0;      /* number of bytes read in input */
    size_t len = 0;         /* size of allocated buffer */

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

        arg = strtok_r(buf, " \n", &save_ptr);

        *skip = (!arg || (arg[0] == COMMENT_CHAR));

        if (!(*skip)) {
            /* Check for instances of $$ that should be expanded. */
            int exp_count = 0;      /* the number of instances of $$ */
            arg = expand_vars(arg, pid, &exp_count);

            if (arg) {
                cmd_parts->cmd = strdup(arg);
                cmd_parts->args[cmd_parts->argsc] = cmd_parts->cmd;
                (cmd_parts->argsc)++;

                /* arg only needs to be freed if replacements were made. */
                if (exp_count > 0) {
                    free(arg);
                }
            } else {
                *skip = true;
            }

            int input_next = -1;    /* whether next arg should be input file */
            int output_next = -1;   /* whether next arg should be output file */

            while (arg && !(*skip)) {
                arg = strtok_r(NULL, " \n", &save_ptr);
                if (arg) {
                    /*
                     * Set cmd_parts->background to false here, since if it was
                     * previously set to true, it would not be last argument.
                     */
                    cmd_parts->background = false;

                    exp_count = 0;
                    arg = expand_vars(arg, pid, &exp_count);

                    long arg_len = strlen(arg);

                    if (!arg) {
                        *skip = true;
                    } else if (input_next == 1) {
                        cmd_parts->input_file = strdup(arg);
                        input_next = 0;
                    } else if (output_next == 1) {
                        cmd_parts->output_file = strdup(arg);
                        output_next = 0;

                    } else if (arg_len == 1 && arg[0] == INPUT_REDIR) {
                    /*
                     * check if current word indicates input redirection
                     * input_next *= -1 will change the value:
                     * -1 -> 1          0 -> 0          1 -> -1
                     * such that, if initialized to -1 and set to 0 after
                     * using, it can only be used once.
                     */
                        input_next *= -1;

                    } else if (arg_len == 1 && arg[0] == OUTPUT_REDIR) {
                        output_next *= -1;

                    } else if (arg_len == 1 && arg[0] == BACKGROUND) {
                        /*
                         * if there are any arguments after this,
                         * cmd_parts->background will be changed to false
                         * near beginning of next iteration of while loop
                         */
                        cmd_parts->background = true;

                    } else {
                        (*skip) = warn_args((cmd_parts->argsc) + 1 > MAX_ARGS,
                                            MAX_ARGS);

                        if (!(*skip)) {
                            cmd_parts->args[cmd_parts->argsc] = strdup(arg);
                            (cmd_parts->argsc)++;
                        }
                    }

                    if (exp_count > 0) {
                        free(arg);
                    }
                }
            }
        }
    }

    free(line);
    line = NULL;
    len = 0;
    return cmd_parts;
}

