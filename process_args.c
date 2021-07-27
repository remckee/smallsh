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
    if (cmd_parts->cmd) {
        printf("command from main: %s\n", print_string_safe(cmd_parts->cmd));
        fflush(NULL);
    } else {
        // error, since the command part is required
    }

    if (cmd_parts->input_file) {
        printf("input file from main: %s\n", print_string_safe(cmd_parts->input_file));
        fflush(NULL);
    } else {
        printf("no input file\n");
        fflush(NULL);
    }

    if (cmd_parts->output_file) {
        printf("output file from main: %s\n", print_string_safe(cmd_parts->output_file));
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
            printf("%d | %s\n", i, print_string_safe(cmd_parts->args[i]));
        } else {
            printf("%d | NULL\n", i);
        }

    }
    //exit(EXIT_SUCCESS);
    return result;
}

bool get_cmd(struct cmd_line *cmd_parts, char *quit) {
    printf("%c ", CMD_PROMPT);
    fflush(NULL);

    char *line = NULL;                  // Stores entire user input, which will be copied
                                        // to buf if it does not exceed the char limit.
    char buf[MAX_CHARS+1];              // MAX_CHARS+1 allows room for \0 at the end
    memset(buf, '\0', sizeof(buf));
    ssize_t nread = 0;
    size_t len = 0;
    //char quit = ' ';

    nread = getline(&line, &len, stdin);

    //struct cmd_line *cmd_parts;
    //cmd_parts = malloc(sizeof(struct cmd_line));
    cmd_parts->cmd = NULL;
    cmd_parts->argsc = 0;
    cmd_parts->input_file = NULL;
    cmd_parts->output_file = NULL;
    cmd_parts->background = false;
    bool skip = false;         // determines whether to skip processing
                               // because line is a comment
                               //

    if ((nread > 1) && line[0]!= COMMENT_CHAR) {
        // If the command line exceeds the character limit, display a warning message
        // to the user, and skip processing the rest of the line.
        skip = warn_chars(nread > MAX_CHARS, MAX_CHARS);

        if (!skip) {
            memset(buf, '\0', sizeof(buf));
            strcpy(buf, line);
            // parse arguments
            char *arg = NULL;
            char *save_ptr;
            //int count = 0;

            // try to parse first argument (command)
            arg = strtok_r(buf, " \n", &save_ptr);

            if (arg) {
                //count++;
                cmd_parts->cmd = arg;
                *quit = cmd_parts->cmd[0];

                int input_next = -1;        // whether the next arg should be an input file
                int output_next = -1;       // whether the next arg should be an output file
                while (arg && !skip) {
                    arg = strtok_r(NULL, " \n", &save_ptr);
                    if (arg) {
                        cmd_parts->background = false;
                        //count++;
                        long arg_len = strlen(arg);
                        // store arg
                        if (input_next==1) {
                            cmd_parts->input_file = arg;
                            input_next = 0;
                        } else if (output_next==1) {
                            cmd_parts->output_file = arg;
                            output_next = 0;

                        // input_next *= -1 will change the value:
                        // -1 -> 1
                        // 0 -> 0
                        // 1 -> -1
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
                            // after incrementing, display a warning message to the user,
                            // and skip processing the rest of the line.
                            skip = warn_args(cmd_parts->argsc+1 > MAX_ARGS, MAX_ARGS);

                            if (!skip) {
                                cmd_parts->args[cmd_parts->argsc] = arg;
                                (cmd_parts->argsc)++;
                            }
                        }
                    }
                }
            }
        }
    }

    free_safe(line);
    return skip;
}

