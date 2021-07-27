#include "smallsh.h"

int main (int argc, char *argv[]) {
    pid_t pid = getpid();
    char *str = "cd$$e q$$x";
    int exp_count = 0;
    str = expand_vars(str, pid, &exp_count);

    printf("%d\n", pid);
    printf("new: %s %d %ld \n", str, exp_count, strlen(str));

    if (exp_count > 0) {
        free_safe(str);
    }

    return 0;
}
