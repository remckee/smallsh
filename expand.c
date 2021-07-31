/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/30/2021
**********************/

#include "smallsh.h"


char *find_replace(char *pattern, char *str, char *repl, int *nrepls) {
    const long len_str = strlen(str);           // original length of str before replacement
    long len_pat = strlen(pattern);             // length of given pattern
    long len_repl = strlen(repl);               // length of replacement string
    char *next_pat = (strstr(str, pattern));    // pointer to start of next instance of pattern
    *nrepls = 0;

    // if next_pat is not NULL, at least one instance of pattern was found
    if (next_pat) {
        long len_new_str = len_str;                 // length of str after replacements

        // If the replacment expands str, calculate length of string after max
        // possible replacments.
        // If the replacement shrinks str (e.g., replacement text is shorter than
        // the pattern), use the original length of str to avoid making new_str smaller
        // than str.
        long max_len = (len_repl <= len_pat) ? len_str : (len_str/len_pat)*(len_repl) + (len_str%len_pat);


        char *new_str = NULL;
        new_str = malloc_safe(new_str, (max_len+1)*sizeof(char));

        char *s = str;                          // pointer to current location within str
        char *end = (str+len_str);              // pointer to end of original str
        long i = 0;                             // index in new_str


        while (s < end) {
            if (!next_pat) {
                new_str[i] = *s;
                s++;
                i++;
            } else {
                (*nrepls)++;

            // Loop through str (via ptr s) character-by-character until an
            // instance of pattern is found.
                while (s < next_pat) {
                    new_str[i] = *s;
                    s++;
                    i++;
                }
                for (int j = 0; j < len_repl; j++) {
                    new_str[i] = repl[j];
                    i++;
                }
                s = (next_pat+len_pat);
                next_pat = strstr(s, pattern);
                len_new_str += (len_repl - len_pat);
            }
        }
        new_str[i] = '\0';
        new_str = realloc(new_str, (i+1) * sizeof(char));
        assert(new_str != NULL);
        str = new_str;
    }
    return str;
}


/* Replaces all instances of PID_VAR in str with pid. */
/* The number of replacements made is stored in nrepls. */
/* Note that a new string will be allocated if any replacements */
/* are made, so the caller will need to free str if nrepls > 0. */
char *expand_vars(char *str, pid_t pid, int *nrepls) {
    int size = num_digits(pid);
    char *result;

    if (size > 0) {
        char pid_ascii[size+1];

        // convert pid to an ascii string and store in pid_ascii
        size = ltoa_dec_buf(pid, pid_ascii, sizeof(pid_ascii));

        if (size > 0) {
            result = find_replace(PID_VAR, str, pid_ascii, nrepls);
        } else {
            result = NULL;
        }

    } else {
        result = NULL;
    }
    return result;
}
