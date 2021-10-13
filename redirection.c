#include "smallsh.h"


// Redirect the given file_name to new_fd with the given flags.
// msg specifies whether it is input or output for the error message.
// Returns -1 upon error.
int redirect(char *file_name, char *msg, int flags, int new_fd) {
    int fd = open(file_name, flags, S_IRW);

    if (fd == -1) {
        // warn about error opening file
        fprintf(stderr, "cannot open %s for %s\n", file_name, msg);
        fflush(stdout);
        exit(FAILURE);
    } else {
        fd = dup2(fd, new_fd);

        // warn about error redirecting
        if (fd == -1) {
            fprintf(stderr, "cannot redirect %s to %s\n", msg, file_name);
            fflush(stdout);
            exit(FAILURE);
        }
    }
    return fd;
}


// Redirect input to file_name. Returns -1 upon error.
int redirect_input(char *file_name) {
    return redirect(file_name, "input", O_RDONLY, STDIN_FILENO);
}


// Redirect output to file_name. Returns -1 upon error.
int redirect_output(char *file_name) {
    return redirect(file_name, "output", O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
}
