#include "smallsh.h"


/*
 * This function finds each occurrence of pattern in str and replaces it with
 * repl. The resulting string is returned, and the number of replacements made
 * is placed in pointer nrepls. The string is allocated only if at least one
 * replacement was made, so the caller must check that nrepls is greater than
 * 0 before calling free on the returned string. If no replacements are made,
 * it returns str unchanged.
 */
char *find_replace(char *pattern, char *str, char *repl, int *nrepls) {
    const long len_str = strlen(str);           // original length of str
    long len_pat = strlen(pattern);             // length of given pattern
    long len_repl = strlen(repl);               // length of replacement string
    char *next_pat = (strstr(str, pattern));    // pointer to start of next
                                                // instance of pattern
    *nrepls = 0;

    // if next_pat is not NULL, at least one instance of pattern was found
    if (next_pat) {
        long len_new_str = len_str; // length of str after replacements

        /*
         * If the replacment expands str (len_repl > len_pat), calculate length
         * of string after max possible replacments.
         * If the replacement shrinks str (e.g., replacement text is shorter
         * than the pattern), use the original length of str to avoid making
         * new_str smaller than str, since we don't yet know how many
         * replacements will be made.
         */
        long max_len = (len_repl > len_pat)
            ? (len_str/len_pat)*(len_repl) + (len_str%len_pat) : len_str;

        // allocate the string to be returned
        char *new_str = NULL;
        new_str = malloc((max_len+1)*sizeof(char));

        // if malloc failed, display an error message and return NULL immediately
        if (warn_error(new_str==NULL, "NULL ptr")) {
            return NULL;
        }

        // Iterate through str to find and replace each instance of pattern.

        char *s = str;              // pointer to current location within str
        char *end = (str+len_str);  // pointer to end of original str
        long i = 0;                 // index in new_str

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
                /*
                 * Increment number of replacements made, and update
                 * the length of the new string.
                 */
                (*nrepls)++;
                len_new_str += (len_repl - len_pat);

            /*
             * Loop through str (via ptr s) character-by-character, copying to
             * new_str, until the next instance of pattern (found previously).
             */
                while (s < next_pat) {
                    new_str[i] = *s;
                    s++;
                    i++;
                }

                // copy the replacement into new_str
                for (int j = 0; j < len_repl; j++) {
                    new_str[i] = repl[j];
                    i++;
                }
                /*
                 * Move s to the char after the pattern, then search for
                 * the next instance of pattern.
                 */
                s = (next_pat+len_pat);
                next_pat = strstr(s, pattern);
            }
        }

        /*
         * Add null terminator, reallocate new_str according to actual
         * size, and assign to str.
         */
        new_str[i] = '\0';
        new_str = realloc(new_str, (i+1) * sizeof(char));
        assert(new_str != NULL);
        str = new_str;
    }
    return str;
}


/*
 * Converts a long in the specified base to a char buffer
 * and returns the number of digits in the number if successful.
 * Returns 0 if it failed.
 */
int ltoa_buf(long num, char *buf, int size, int base) {
    // clear buffer; avoiding possibly nonreentrant function memset
    for (int k = 0; k < size; k++) {
        buf[k] = '\0';
    }

    /*
     * If num is negative, place the sign in buf[0] and make num positive.
     * Increment the variable start so that the sign will not be overwritten
     * when the chars are copied within the array later.
     */
    int start = 0;
    if (num < 0) {
        buf[0] = '-';
        num *= -1;
        start++;
    }

    /*
     * Determine the value of each digit of num, starting at the right-most
     * digit of num, and copy it from right to left to buf,
     * starting at second-to-last char.
     */

    int digit;                  // value of current digit of num
    int num_digits = 0;         // used to keep track of the number of digits
    int i = size-1;             // index within buf
    buf[i] = '\0';
    i--;

    // special case for num == 0 since it will not enter the while loop
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

    /*
     * move digits to beginning of array, clearing each src digit after
     * it is copied to dest
     * avoiding possibly nonreentrant function memmove
     */
    for (int dest = start, src = i+1; src < size && dest < src; dest++, src++) {
        buf[dest] = buf[src];
        buf[src] = '\0';
    }

    // clearing rest of array, just in case it wasn't already cleared
    for (int j = num_digits; j < size; j++) {
        buf[start+j] = '\0';
    }

    return num_digits;
}


/*
 * decimal long to ascii conversion
 * Returns the number of digits in the number if successful.
 * Returns 0 if it failed.
 */
int ltoa_dec_buf(long num, char *buf, int size) {
    return ltoa_buf(num, buf, size, 10);
}


/*
 * Replaces all instances of PID_VAR in str with pid.
 * The number of replacements made is stored in nrepls.
 * Note that a new string will be allocated if any replacements
 * are made, so the caller will need to free str if nrepls > 0.
 * Returns the allocated string if any replacements were made
 * Returns an unallocated string identical to str if no replacements were made
 * Return NULL if an error occurred
 */
char *expand_vars(char *str, char *pid, int *nrepls) {
    char *result;

    // If pid has at least 1 char, replace all instances of PID_VAR with pid
    if (strlen(pid) > 0) {
        result = find_replace(PID_VAR, str, pid, nrepls);
    } else {
        result = NULL;
    }

    return result;
}


/*
 * A version of expand_vars that takes an unconverted, integer (pid_t) argument
 * for pid. Note that this function is not used in the program currently.
 *
 * There are two versions because expand_vars_num was the original
 * version, but using that required the same pid value to be converted to a
 * string each time it was used in a run of smallsh, which seemed inefficient.
 * I decided to leave this version in the program in case it would be useful.
 * Return value and functionality are the same as expand_vars()
 */
char *expand_vars_num(char *str, pid_t pid, int *nrepls) {
    int size = 21;  // Max number of characters for a long:
                    // 9223372036854775807 has 19 chars + 2 for sign and \0

    char *result;
    char pid_ascii[size];   // used to store the assci characters of pid

    // convert pid to an ascii string and store in pid_ascii
    size = ltoa_dec_buf(pid, pid_ascii, size);

    // If conversion was successful, replace all instances of PID_VAR with pid
    if (size > 0) {
        result = find_replace(PID_VAR, str, pid_ascii, nrepls);
    } else {
        result = NULL;
    }

    return result;
}

