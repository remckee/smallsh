#include "smallsh.h"


/**
 * find_replace():
 * Find each occurrence of @pattern in @str and replace it with @repl.
 * @pattern: the exact string to search for. Cannot handle regex patterns.
 * @str: the string to search
 * @repl: the string to replace @pattern with
 * @nrepls: an output variable for the number of replacements made
 *
 * Returns:
 * the resulting string after replacements are made. The string is allocated
 * only if at least one replacement was made, so the caller must check that
 * @nrepls is greater than 0 before calling free on the returned string. If no
 * replacements are made, it returns @str unchanged.
 */
char *find_replace(char *pattern, char *str, char *repl, int *nrepls) {
    const long len_str = strlen(str);           /* original length of str */
    long len_pat = strlen(pattern);
    long len_repl = strlen(repl);
    char *next_pat = (strstr(str, pattern));    /* pointer to start of next */
                                                /* instance of pattern */
    *nrepls = 0;

    if (next_pat) {
        long len_new_str = len_str; /* length of str after replacements */

        /*
         * If the replacment expands str, calculate length of string after max
         * possible replacments. If the replacement shrinks str, use the
         * original length of str.
         */
        long max_len = (len_repl > len_pat)
            ? (len_str / len_pat) * (len_repl) + (len_str % len_pat) : len_str;

        /* allocate the string to be returned */
        char *new_str = NULL;

        new_str = malloc((max_len + 1) * sizeof(char));
        if (warn_error(new_str == NULL, "NULL ptr")) {
            return NULL;
        }

        char *s = str;              /* pointer to current location within str */
        char *end = (str + len_str);/* pointer to end of original str */
        long i = 0;                 /* index in new_str */

        while (s < end) {
            if (!next_pat) {
                /*
                 * If there are no more instances of pattern,
                 * copy the rest of str into new_str
                 */
                new_str[i] = *s;
                s++;
                i++;
            } else {
                (*nrepls)++;
                len_new_str += (len_repl - len_pat);

                /* Loop through str until the next instance of pattern */
                while (s < next_pat) {
                    new_str[i] = *s;
                    s++;
                    i++;
                }

                for (int j = 0; j < len_repl; j++) {
                    new_str[i] = repl[j];
                    i++;
                }
                /*
                 * Move s to the char after the pattern, then search for
                 * the next instance of pattern.
                 */
                s = (next_pat + len_pat);
                next_pat = strstr(s, pattern);
            }
        }

        /*
         * Add null terminator, reallocate new_str according to actual
         * size, and assign to str.
         */
        new_str[i] = '\0';
        new_str = realloc(new_str, (i + 1) * sizeof(char));
        assert(new_str != NULL);
        str = new_str;
    }
    return str;
}


/**
 * ltoa_buf():
 * Convert a numerical value to a string.
 * @num: the number to convert
 * @buf: out parameter for the string representation of @num
 * @size: length of @buf
 * @base: the base of @num
 *
 * Return:
 * the number of digits in the number if successful, 0 if it failed
 */
int ltoa_buf(long num, char *buf, int size, int base) {
    /* clear buffer, avoiding possibly nonreentrant function memset */
    for (int k = 0; k < size; k++) {
        buf[k] = '\0';
    }

    int start = 0;
    if (num < 0) {
        buf[0] = '-';
        num *= -1;
        start++;
    }

    int digit;                  /* value of current digit of num */
    int num_digits = 0;         /* used to keep track of the number of digits */
    int i = size - 1;           /* index within buf */
    buf[i] = '\0';
    i--;

    if (num == 0) {
        buf[i] = 0x30;
        num_digits++;
        i--;
    }

    while (num > 0 && i >= 0) {
        digit = num % base;
        buf[i] = 0x30 + digit;
        num /= base;
        num_digits++;
        i--;
    }

    /* move digits to beginning of array */
    for (int dest = start, src = i + 1; src < size && dest < src;
         dest++, src++) {
        buf[dest] = buf[src];
        buf[src] = '\0';
    }

    for (int j = num_digits; j < size; j++) {
        buf[start + j] = '\0';
    }

    return num_digits;
}


/**
 * ltoa_dec_buf():
 * Convert a base 10 numerical value to a string.
 * @num: the number to convert
 * @buf: out parameter for the string representation of @num
 * @size: length of @buf
 *
 * Return:
 * the number of digits in the number if successful, 0 if it failed
 */
int ltoa_dec_buf(long num, char *buf, int size) {
    return ltoa_buf(num, buf, size, 10);
}


/**
 * expand_vars():
 * Replace all instances of PID_VAR in a string with a given pid.
 * @str: the string to search
 * @pid: the pid to replace PID_VAR with, in string form
 * @nrepls: the number of replacements made. Note that a new string will be
 *          allocated if any replacements are made, so the caller will need to
 *          free @str if @nrepls > 0.
 *
 * Return:
 * - the allocated string if any replacements were made
 * - an unallocated string identical to @str if no replacements were made
 * - NULL if an error occurred
 */
char *expand_vars(char *str, char *pid, int *nrepls) {
    char *result;

    if (strlen(pid) > 0) {
        result = find_replace(PID_VAR, str, pid, nrepls);
    } else {
        result = NULL;
    }

    return result;
}

