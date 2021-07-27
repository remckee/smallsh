/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/26/2021
**********************/

#include "smallsh.h"


void *malloc_safe(void *ptr, size_t size) {
    void *loc_ptr = malloc(size);

    if(loc_ptr) {
        ptr = loc_ptr;
    } else {
        assert(loc_ptr!=NULL);
    }
    return ptr;
}

void free_safe(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

char *print_string_safe(char *str) {
    return (str) ? str : "NULL";
}

bool warn_chars(bool condition, int max_chars) {
    bool result = false;
    if (condition) {
        printf("The command line was too long. A line can have a maximum of %d characters.\n", max_chars);
        fflush(NULL);
        result = true;
    }
    return result;
}

bool warn_args(bool condition, int max_args) {
    bool result = false;
    if (condition) {
        printf("Too many command line arguments. A line can have a maximum of %d arguments.\n", max_args);
        fflush(NULL);
        result = true;
    }
    return result;
}

// converts a long in the specified base to a char buffer
int ltoa_buf(long num, char *buf, int size, int base) {
    int len = (log10(num) / log10(base)) + 1;
    if (len > size) {

        len = -1;
    } else {
        long divisor;
        long rem;
        int digit;

        for (int i = 0, l = len; l > 0; l--, i++) {
            divisor = pow(base, l-1);
            rem = num % divisor;
            digit = num / divisor;
            buf[i] = 0x30 + digit;
            num = rem;
        }
        buf[len] = '\0';
    }
    return len;
}

// decimal long to ascii conversion
int ltoa_dec_buf(long num, char *buf, int size) {
    return ltoa_buf(num, buf, size, 10);
}

// returns the new length of str after replacements
// Partial based on:
// https://man7.org/linux/man-pages/man3/regexec.3.html#EXAMPLES
long find_replace(const char *const pattern, const char *const str, char *replace) {
    regex_t regex;
    regmatch_t pmatch[1];
    regoff_t offset;
    long len_str = strlen(str);
    long len_pat = 0;// = strlen(pattern);
    long len_repl = strlen(replace);
    const char *s = str;
    int count = 0;
    //char *new_str = NULL;

if (regcomp(&regex, pattern, REG_NEWLINE)) {
    assert(!regcomp(&regex, pattern, REG_NEWLINE));
    //exit(1);
} else {

    for (int i = 0; !regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0); i++, count++) {
        len_pat = pmatch[0].rm_eo - pmatch[0].rm_so;
        offset = pmatch[0].rm_so + (s - str);
        printf("%d:\n", i);
        printf("offset = %jd\n", (intmax_t)offset);
        printf("substring = \"%.*s\"\n", (int)len_pat, s + pmatch[0].rm_so);
        printf("%s\n", s);

        len_str = len_str - len_pat + len_repl;
        s += pmatch[0].rm_eo;
    }
}
    printf("len str: %ld len pat: %ld len repl: %ld\n", len_str, len_pat, len_repl);



    regfree(&regex);
    return len_str;
}

//
char *find_replace_str(char *pattern, char *str, char *repl, int *nrepls) {
    const long len_str = strlen(str);           // the original length of str before expansion
    long len_pat = strlen(pattern);             // length of given pattern
    long len_repl = strlen(repl);               // length of replacement string
    char *next_pat = (strstr(str, pattern));    // pointer to start of next instance of pattern
    *nrepls = 0;

    // if next_pat is not NULL, at least one instance of pattern was found
    if (next_pat) {
        long len_new_str = len_str;                 // length of str after expansions

        // calculate length of string after max possible expansions
        long max_len = (len_str/len_pat)*(len_repl) + (len_str%len_pat);
        //printf("%ld\n", max_len);
        char *new_str = NULL;
        new_str = malloc_safe(new_str, (max_len+1)*sizeof(char));

        char *s = str;                          // pointer to current location within str
        char *end = (str+len_str);              // pointer to end of original str
        long i = 0;                             // index in new_str


        while (s < end) {
            if (!next_pat) {
                //printf("%c", *s);
                new_str[i] = *s;
                s++;
                i++;
            } else {
                (*nrepls)++;
                while (s < next_pat) {
                    //printf("%c", *s);
                    new_str[i] = *s;
                    s++;
                    i++;
                }
                //printf("%s", repl);
                for (int j = 0; j < len_repl; j++) {
                    new_str[i] = repl[j];
                    i++;
                }
                s = (next_pat+len_pat);
                next_pat = strstr(s, pattern);
                len_new_str = len_new_str - len_pat + len_repl;
            }
        }
        new_str[i] = '\0';
        new_str = realloc(new_str, (i+1) * sizeof(char));
        assert(new_str != NULL);
        //if (len_str < len_new_str) {
        str = new_str;
       // }

        printf("\nnew str: %s\n", str);
    }
    return str;
}


char *expand_vars(char *str, pid_t pid, int *nrepls) {
    int size = (log10(pid) / log10(10)) + 2;
    char buf[size];
    ltoa_buf(pid, buf, sizeof(buf), 10);
    return find_replace_str(PID_VAR, str, buf, nrepls);
}
