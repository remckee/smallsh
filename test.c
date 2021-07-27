#include "smallsh.h"

int main (int argc, char *argv[]) {
    pid_t pid = getpid();
    char *str = "cd$$e q$$x";
    char *pat = "$$";
    int size = (log10(pid) / log10(10)) + 2;
    char buf[size];

    ltoa_buf(pid, buf, sizeof(buf), 10);
    long new_len = find_replace_str(pat, str, buf);

    printf("%d\n", pid);
    printf("%ld\n", new_len);

    //int loc = strstr(str, "$$");
    //printf("loc:%s\n", strstr(str, "$$"));
    printf("new: %s %ld", str, strlen(str));

    return 0;
}
