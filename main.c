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

   // char *line = NULL;
    //size_t len = 0;
    ssize_t nread;
    char *line = NULL;//[MAX_CMD_CHARS+2];
    size_t len = 0;
    line = malloc(sizeof(char)*(MAX_CMD_CHARS+2));

    /* if (argc != 2) { */
    /*    fprintf(stderr, "Usage: %s <file>\n", argv[0]); */
    /*    exit(EXIT_FAILURE); */
    /* } */

    //while ((line = fgets(line, MAX_CMD_CHARS+1, stdin)) != NULL) {
    while ((nread = getline(&line, &len, stdin)) != -1) {
        if (nread > MAX_CMD_CHARS+1) {
            printf("The command line was too long. A line can have a maximum of %d characters.\n", MAX_CMD_CHARS);
        } else {
            printf("Retrieved line of length %zd:\n", nread);
            fwrite(line, nread, 1, stdout);
            //puts(line);
        }

        printf("%c ", CMD_PROMPT);
    }

    free(line);
    //free(cmd);
    //fflush(NULL);
    //execlp()

    //

    //perror("execv");   /* execve() returns only on error */
    //exit(EXIT_FAILURE);
    return 0;
}
