#include "smallsh.h"


/**
 * redirect():
 * Redirect input or output to a file.
 * @file_name: name of the file to redirect to
 * @msg: a string specifying whether it is input or output for use in the error
 *       message
 * @flags: flags to use when opening the file
 * @new_fd: the file descriptor that will be redirected
 *
 * Returns:
 * the file descriptor of @file_name or -1 upon error
 */
int redirect(char *file_name, char *msg, int flags, int new_fd) {
    int fd = open(file_name, flags, S_IRW);

    if (fd == -1) {
        fprintf(stderr, "cannot open %s for %s\n", file_name, msg);
        fflush(stdout);
        exit(FAILURE);
    } else {
        fd = dup2(fd, new_fd);

        if (fd == -1) {
            fprintf(stderr, "cannot redirect %s to %s\n", msg, file_name);
            fflush(stdout);
            exit(FAILURE);
        }
    }
    return fd;
}


/**
 * redirect_input():
 * Redirect input to @file_name.
 * @file_name: name of the file to redirect to
 *
 * Returns:
 * the file descriptor of @file_name or -1 upon error
 */
int redirect_input(char *file_name) {
    return redirect(file_name, "input", O_RDONLY, STDIN_FILENO);
}


/**
 * redirect_output():
 * Redirect output to @file_name.
 * @file_name: name of the file to redirect to
 *
 * Returns:
 * the file descriptor of @file_name or -1 upon error
 */
int redirect_output(char *file_name) {
    return redirect(file_name, "output", O_WRONLY | O_CREAT | O_TRUNC,
                    STDOUT_FILENO);
}
