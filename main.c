/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/22/2021
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
   // char *line = NULL;
    //size_t len = 0;
    ssize_t nread = 0;
    char *line = NULL;//[MAX_CMD_CHARS+2];
    size_t len = 0;
    struct cmd_line *cmd_parts;
    cmd_parts = malloc(sizeof(struct cmd_line));

    char quit = ' ';
    //line = malloc(sizeof(char)*(MAX_CMD_CHARS+2));

    /* if (argc != 2) { */
    /*    fprintf(stderr, "Usage: %s <file>\n", argv[0]); */
    /*    exit(EXIT_FAILURE); */
    /* } */
//EINVAL
    //while ((line = fgets(line, MAX_CMD_CHARS+1, stdin)) != NULL) {
    while (quit !='q' &&(nread = getline(&line, &len, stdin)) != -1) {
        if (nread > 1) {
            // parse arguments
            char *arg = NULL;
            int index = 0;
            int count = 0;
            int diff;
            bool skip = false;         // determines whether to skip processing
                                       // because line is a comment
            sscanf(&line[index], "%ms%n", &arg, &diff);
            index += diff;

            if (arg) {
                count++;
                printf("arg %s of length %d. Total arg count: %d\n", arg, diff, count);
                fflush(NULL);
                // process arg
                cmd_parts->cmd = arg;//malloc(sizeof(arg));
                //strcpy(cmd_parts->cmd, arg);
                //free(arg);
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
                    } else {
                        while (arg && index < nread && count <= MAX_CMD_ARGS) {
                            sscanf(&line[index], "%ms%n", &arg, &diff);
                            index += diff;
                            if (arg) {
                                count++;

                                if (count > MAX_CMD_ARGS) {
                                    printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", MAX_CMD_ARGS);
                                    fflush(NULL);
                                } else {
                                    printf("arg %s of length %d. Total arg count: %d\n", arg, diff, count);
                                    fflush(NULL);
                                    // process arg
                                }

                                free(arg);

                            }
                        }
                    }

                    //fwrite(line, nread, 1, stdout);
                    //puts(line);
                }
                skip = false;
                free(cmd_parts->cmd);
            }
        }


        printf("%c ", CMD_PROMPT);
        fflush(NULL);
    }

    free(cmd_parts);
    free(line);
    //execlp()
    //perror("execv");   /* execve() returns only on error */
    //exit(EXIT_FAILURE);
    return 0;
}
