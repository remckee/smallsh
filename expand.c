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


// converts a long in the specified base to a char buffer
// and returns the number of digits in the number if successful.
int ltoa_buf(long num, char *buf, int size, int base) {
    // clear buffer; avoiding possibly nonreentrant function memset
    for (int k = 0; k < size; k++) {
        buf[k] = '\0';
    }

    // If num is negative, place the sign in buf[0] and make num positive.
    // Increment the variable start so that the sign will not be overwritten
    // when the chars are copied within the array later.
    int start = 0;
    if (num < 0) {
        buf[0] = '-';
        num *= -1;
        start++;
    }

    int digit;                  // value of current digit of num
    int num_digits = 0;         // used to keep track of the number of digits
    int i = size-1;             // index within buf
    buf[i] = '\0';
    i--;

    // special case for num == 0 since it will not enter the for loop
    if (num == 0) {
        buf[i] = 0x30;
        num_digits++;
        i--;
    }

    // Determine the value of each digit of num, starting at the right-most
    // digit of num, and copy it from right to left to buf,
    // starting at second-to-last char.
    for ( ; num > 0 && i >= 0; num_digits++, i--) {
        digit = num % base;
        buf[i] = 0x30 + digit;
        num /= base;
    }

    // move digits to beginning of array, clearing each src digit after
    // it is copied to dest
    // avoiding possibly nonreentrant function memmove
    for (int dest = start, src = i+1; src < size; dest++, src++) {
        assert(dest < src);
        buf[dest] = buf[src];
        buf[src] = '\0';
    }

    // clearing rest of array
    for (int j = num_digits; j < size; j++) {
        buf[start+j] = '\0';
    }

    return num_digits;
}


// decimal long to ascii conversion
// Returns the number of digits in the number if
// successful. It returns -1 on error, so caller MUST check
// that the result is positive before using.
int ltoa_dec_buf(long num, char *buf, int size) {
    return ltoa_buf(num, buf, size, 10);
}


/* Replaces all instances of PID_VAR in str with pid. */
/* The number of replacements made is stored in nrepls. */
/* Note that a new string will be allocated if any replacements */
/* are made, so the caller will need to free str if nrepls > 0. */
char *expand_vars(char *str, pid_t pid, int *nrepls) {
    int size = 21; // 9223372036854775807 has 19 chars + 2 for sign and \0
    char *result;

  //  if (size > 0) {
    char pid_ascii[size];

    // convert pid to an ascii string and store in pid_ascii
    size = ltoa_dec_buf(pid, pid_ascii, size);

    if (size > 0) {
        result = find_replace(PID_VAR, str, pid_ascii, nrepls);
    } else {
        result = NULL;
    }

   // } else {
  //      result = NULL;
 //   }
    return result;
}
