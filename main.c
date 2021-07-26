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
   // char *line = NULL;
  //  size_t len = 0;
  //  ssize_t nread;

    //newargv[0] = argv[1];
    //char *cmd;
    printf("%c ", CMD_PROMPT);
    fflush(NULL);
   // printf("%c ", CMD_PROMPT);
   // fflush(NULL);
   // char *line = NULL;
    //size_t len = 0;
    ssize_t nread = 0;
    char *line = NULL;//[MAX_CMD_CHARS+2];
    size_t len = 0;
    //cmd_parts->newargv = malloc(sizeof(char) * MAX_CMD_ARGS);

    char quit = ' ';
    //line = malloc(sizeof(char)*(MAX_CMD_CHARS+2));

    /* if (argc != 2) { */
    /*    fprintf(stderr, "Usage: %s <file>\n", argv[0]); */
    /*    exit(EXIT_FAILURE); */
    /* } */
//EINVAL
    //while ((line = fgets(line, MAX_CMD_CHARS+1, stdin)) != NULL) {
    while (quit !='q' &&(nread = getline(&line, &len, stdin)) != -1) {
        struct cmd_line *cmd_parts;
        cmd_parts = malloc(sizeof(struct cmd_line));
        cmd_parts->cmd = NULL;
        cmd_parts->input_file = NULL;
        cmd_parts->output_file = NULL;
        cmd_parts->background = false;


        if (nread > 1) {
            // parse arguments
            char *arg = NULL;
            int index = 0;
            int count = 0;
            cmd_parts->argsc = 0;
            int diff;
            bool skip = false;         // determines whether to skip processing
                                       // because line is a comment
                                       //
            // try to parse first argument (command)
            sscanf(&line[index], "%ms%n", &arg, &diff);
            index += diff;

            if (arg) {
                count++;
                printf("arg %s of length %d. Total arg count: %d\n", arg, diff, count);
                fflush(NULL);
                // process arg
                cmd_parts->cmd = strdup(arg);//malloc(sizeof(arg));
                //strcpy(cmd_parts->cmd, arg);
                free(arg);
                if (cmd_parts->cmd[0] == COMMENT_CHAR) {
                    skip = true;
                }
                quit = cmd_parts->cmd[0];
                printf("command: %s\n", cmd_parts->cmd);
                fflush(NULL);


                // if line was not a comment
                if (!skip) {
                    if (nread > MAX_CMD_CHARS+1) {
                        printf("The command line was too long. A line can have a maximum of %d characters.\n", MAX_CMD_CHARS);
                        fflush(NULL);
                        skip = true;
                    } else {
                        int input_next = -1;        // whether the next arg should be an input file
                        int output_next = -1;       // whether the next arg should be an output file
                        while (arg && index < nread && count <= MAX_CMD_ARGS) {
                            sscanf(&line[index], "%ms%n", &arg, &diff);
                            index += diff;
                            if (arg) {
                                cmd_parts->background = false;

                                count++;
                                long arg_len = strlen(arg);
                                if (count > MAX_CMD_ARGS) {
                                    printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", MAX_CMD_ARGS);
                                    fflush(NULL);
                                    skip = true;
                                } else {
                                    printf("arg %s of length %ld. Total arg count: %d\n", arg, arg_len, count);
                                    fflush(NULL);
                                    // store arg
                                    if (input_next==1) {
                                        cmd_parts->input_file = strdup(arg);

                                        printf("input file: %s\n", cmd_parts->input_file);
                                        fflush(NULL);
                                        input_next = 0;
                                    } else if (output_next==1) {
                                        cmd_parts->output_file = strdup(arg);

                                        printf("output file: %s\n", cmd_parts->output_file);
                                        fflush(NULL);
                                        output_next = 0;
                                    } else if (arg_len==1) {

                                        // input_next *= -1 will change the value:
                                        // -1 -> 1
                                        // 0 -> 0
                                        // 1 -> -1
                                        // such that, if initialized to -1 and set to 0 after
                                        // using, it can only be used once.
                                        // Works similarly for output_next.
                                        if (arg[0]==INPUT_REDIR) {
                                            input_next *= -1;
                                        } else if (arg[0]==OUTPUT_REDIR) {
                                            output_next *= -1;
                                        } else if (arg[0]==BACKGROUND) {
                                            // if there are any arguments after this,
                                            // cmd_parts->background will be changed to false
                                            // near beginning of next iteration of while loop
                                            cmd_parts->background = true;
                                        }
                                    } else {
                                        // process as part of [arg1 arg2 ...]
                                        (cmd_parts->argsc)++;

                                    }
                                }
                                free(arg);
                            }
                        }
                        if (!skip) {
                            //process args
                            process(cmd_parts);
                        }
                    }

                    //fwrite(line, nread, 1, stdout);
                    //puts(line);
                }
                skip = false;


                if (cmd_parts->input_file) {
                    free(cmd_parts->input_file);
                }

                if (cmd_parts->output_file) {
                    free(cmd_parts->output_file);
                }

                free(cmd_parts->cmd);
            }
        }

        free(cmd_parts);
        printf("%c ", CMD_PROMPT);
        fflush(NULL);
    }


    free(line);
    //execlp()
    //perror("execv");   /* execve() returns only on error */
    //exit(EXIT_FAILURE);
    return 0;
}
