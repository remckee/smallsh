/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/26/2021
**********************/

#include "smallsh.h"

int main (int argc, char *argv[]) {
    //char *newargv[] = { NULL, "hello", "world", NULL };

    /* if (argc != 2) { */
    /*     fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    /* printf("%c ", CMD_PROMPT); */
    /* fflush(NULL); */
    /* char *line = NULL;                  // Stores entire user input, which will be copied */
    /*                                     // to buf if it does not exceed the char limit. */
    /* char buf[MAX_CHARS+1];              // MAX_CHARS+1 allows room for \0 at the end */
    /* memset(buf, '\0', sizeof(buf)); */
    /* ssize_t nread = 0; */
    /* size_t len = 0; */
    char quit = ' ';

    /* if (argc != 2) { */
    /*    fprintf(stderr, "Usage: %s <file>\n", argv[0]); */
    /*    exit(EXIT_FAILURE); */
    /* } */

    //while (quit !='q') {
    while (quit !='q') {
        //(fgets(line, MAX_CHARS+1, stdin));
        struct cmd_line *cmd_parts;
        cmd_parts = malloc(sizeof(struct cmd_line));
        bool skip = get_cmd(cmd_parts, &quit);

        if (!skip) {
            //process args
            print_cmd(cmd_parts);
        }
        /* cmd_parts->cmd = NULL; */
        /* cmd_parts->argsc = 0; */
        /* cmd_parts->input_file = NULL; */
        /* cmd_parts->output_file = NULL; */
        /* cmd_parts->background = false; */
        /* bool skip = false;         // determines whether to skip processing */
        /*                            // because line is a comment */
        /*                            // */

        /* if ((nread > 1) && line[0]!= COMMENT_CHAR) { */
        /*     // If the command line exceeds the character limit, display a warning message */
        /*     // to the user, and skip processing the rest of the line. */
        /*     skip = warn_chars(nread > MAX_CHARS, MAX_CHARS); */

        /*     if (!skip) { */
        /*         memset(buf, '\0', sizeof(buf)); */
        /*         strcpy(buf, line); */
        /*         // parse arguments */
        /*         char *arg = NULL; */
        /*         char *save_ptr; */
        /*         //int count = 0; */

        /*         // try to parse first argument (command) */
        /*         arg = strtok_r(buf, " \n", &save_ptr); */

        /*         if (arg) { */
        /*             //count++; */
        /*             cmd_parts->cmd = arg; */
        /*             quit = cmd_parts->cmd[0]; */

        /*             int input_next = -1;        // whether the next arg should be an input file */
        /*             int output_next = -1;       // whether the next arg should be an output file */
        /*             while (arg && !skip) { */
        /*                 arg = strtok_r(NULL, " \n", &save_ptr); */
        /*                 if (arg) { */
        /*                     cmd_parts->background = false; */
        /*                     //count++; */
        /*                     long arg_len = strlen(arg); */
        /*                     // store arg */
        /*                     if (input_next==1) { */
        /*                         cmd_parts->input_file = arg; */
        /*                         input_next = 0; */
        /*                     } else if (output_next==1) { */
        /*                         cmd_parts->output_file = arg; */
        /*                         output_next = 0; */

        /*                     // input_next *= -1 will change the value: */
        /*                     // -1 -> 1 */
        /*                     // 0 -> 0 */
        /*                     // 1 -> -1 */
        /*                     // such that, if initialized to -1 and set to 0 after */
        /*                     // using, it can only be used once. */
        /*                     // Works similarly for output_next. */
        /*                     } else if (arg_len==1 && arg[0]==INPUT_REDIR) { */
        /*                         input_next *= -1; */
        /*                     } else if (arg_len==1 && arg[0]==OUTPUT_REDIR) { */
        /*                         output_next *= -1; */
        /*                     } else if (arg_len==1 && arg[0]==BACKGROUND) { */
        /*                         // if there are any arguments after this, */
        /*                         // cmd_parts->background will be changed to false */
        /*                         // near beginning of next iteration of while loop */
        /*                         cmd_parts->background = true; */
        /*                     } else { */
        /*                         // If the command line args will exceed the argument limit */
        /*                         // after incrementing, display a warning message to the user, */
        /*                         // and skip processing the rest of the line. */
        /*                         skip = warn_args(cmd_parts->argsc+1 > MAX_ARGS, MAX_ARGS); */

        /*                         if (!skip) { */
        /*                             cmd_parts->args[cmd_parts->argsc] = arg; */
        /*                             (cmd_parts->argsc)++; */
        /*                         } */
        /*                     } */
        /*                 } */
        /*             } */
        /*             if (!skip) { */
        /*                 //process args */
        /*                 print_cmd(cmd_parts); */
        /*             } */
        /*         } */
        /*     } */
        /* } */
        /* free_safe(cmd_parts); */
        /* printf("%c ", CMD_PROMPT); */
        /* fflush(NULL); */
        free_safe(cmd_parts);
    }

    //free_safe(line);
    //execlp()
    //perror("execv");   /* execve() returns only on error */
    //exit(EXIT_FAILURE);
    //

    return 0;
}
