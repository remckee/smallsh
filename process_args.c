#include "smallsh.h"


// Returns the given string if not NULL or the text "NULL" if NULL
char *print_string_safe(char *str) {
    return (str) ? str : "NULL";
}


// Prints the parsed parts of the command stored in the given struct.
void print_cmd(struct cmd_line *cmd_parts) {
    // print the command
    if (cmd_parts->cmd) {
        printf("command: %s ", print_string_safe(cmd_parts->cmd));
        fflush(stdout);
    } else {
        assert(cmd_parts->cmd);
    }

    // print the input file, if any
    if (cmd_parts->input_file) {
        printf("input file: %s ", print_string_safe(cmd_parts->input_file));
        fflush(stdout);
    } else {
        printf("no input file ");
        fflush(stdout);
    }

    // print the output file, if any
    if (cmd_parts->output_file) {
        printf("output file: %s ", print_string_safe(cmd_parts->output_file));
        fflush(stdout);
    } else {
        printf("no output file ");
        fflush(stdout);
    }

    // print fg/bg status
    if (cmd_parts->background) {
        printf("background\n");
        fflush(stdout);
    } else {
        printf("foreground\n");
        fflush(stdout);
    }

    // print the elements of the args array
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


// Returns a bool indicating whether this command should be skipped because
// the read was invalid or only contained a newline char, it was a comment,
// or it exceeded the char limit.
bool valid_line(char *line, ssize_t nread) {
    bool skip = false;

    // Clear any errors from stdin.
    if (ferror(stdin)) {
        clearerr(stdin);
        skip = true;
    }

    // If the command line exceeds the character limit, warn_chars will display a
    // warning message to the user and return true.
    skip = (nread <= 1) || (line[0]==COMMENT_CHAR) || warn_chars(nread > MAX_CHARS+1, MAX_CHARS);

    return skip;
}


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


// Parse the given command line into a struct and return the struct.
// If the command should not be processed for any reason, the bool pointer
// skip is set to true.
struct cmd_line *get_cmd(bool *skip, char *pid) {
    // print the command prompt
    printf("%c ", CMD_PROMPT);
    fflush(stdout);

    char *line = NULL;          // Stores entire user input, which will be copied
                                // to buf if it does not exceed the char limit.
    char buf[MAX_CHARS+2];      // MAX_CHARS+2 allows room for \n and \0 at the end
    ssize_t nread = 0;          // number of bytes read in input
    size_t len = 0;             // size of allocated buffer

    // read in and validate line
    nread = getline(&line, &len, stdin);
    *skip = valid_line(line, nread);

    // allocate and initialize cmd_line struct
    struct cmd_line *cmd_parts = NULL;
    cmd_parts = malloc(sizeof(struct cmd_line));
    init_cmd_struct(cmd_parts);

    if (!(*skip)) {
        // clear buf, and copy line into it
        memset(buf, '\0', sizeof(buf));
        strcpy(buf, line);
        char *arg = NULL;
        char *save_ptr;

        // try to parse first argument (command)
        arg = strtok_r(buf, " \n", &save_ptr);

        // Skip rest of command if arg is NULL (possibly due to containing only
        // whitespace) or command is a comment line not caught previously.
        // Note that a comment line with leading whitespace, if it exceeds the char limit,
        // should be caught by warn_chars, not here, to avoid buffer overflow.
        *skip = !arg || (arg[0]==COMMENT_CHAR);

        if (!(*skip)) {
            // Check for instances of $$ that should be expanded.
            // expand_vars will return the new string after the expansions.
            int exp_count = 0;                          // the number of instances of $$
            arg = expand_vars(arg, pid, &exp_count);

            // If arg is not NULL, copy the command into the cmd member of
            // the struct, and into the first element of the args array.
            if (arg) {
                cmd_parts->cmd = strdup(arg);
                cmd_parts->args[cmd_parts->argsc] = cmd_parts->cmd;
                (cmd_parts->argsc)++;

                // expand_vars returned the number of $$ replacements made in
                // exp_count. If no replacements were made, it will be zero.
                // arg will only need to be freed if replacements were made.
                if (exp_count > 0) {
                    free(arg);
                }
            } else {
                // If arg is NULL, skip the rest of the command.
                *skip = true;
            }

            int input_next = -1;        // whether the next arg should be an input file
            int output_next = -1;       // whether the next arg should be an output file

            // process the rest of the command, if any
            while (arg && !(*skip)) {
                // get next word
                arg = strtok_r(NULL, " \n", &save_ptr);
                if (arg) {
                    // Set cmd_parts->background to false here, since if it was
                    // previously set to true, it would not be the last argument.
                    cmd_parts->background = false;

                    // expand instances of $$
                    exp_count = 0;
                    arg = expand_vars(arg, pid, &exp_count);

                    // length of current word
                    long arg_len = strlen(arg);

                    if (!arg) {
                        *skip = true;
                    } else if (input_next==1) {
                        // if the last word processed indicated input redirection,
                        // stored this word in the struct as the input file
                        cmd_parts->input_file = strdup(arg);
                        input_next = 0;
                    } else if (output_next==1) {
                        // store as output file
                        cmd_parts->output_file = strdup(arg);
                        output_next = 0;

                    } else if (arg_len==1 && arg[0]==INPUT_REDIR) {
                    // check if current word indicates input redirection
                    // input_next *= -1 will change the value:
                    // -1 -> 1          0 -> 0          1 -> -1
                    // such that, if initialized to -1 and set to 0 after
                    // using, it can only be used once.
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

                        // store as in args array
                        if (!(*skip)) {
                            cmd_parts->args[cmd_parts->argsc] = strdup(arg);
                            (cmd_parts->argsc)++;
                        }
                    }
                    // free any memory allocated by expand_vars
                    if (exp_count > 0) {
                        free(arg);
                    }
                }
            }
        }
    }

    // free input line
    free(line);
    line = NULL;
    len = 0;
    return cmd_parts;
}

