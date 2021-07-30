/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/29/2021
**********************/

#include "smallsh.h"


// returns a positive value on success
// returns a negative value on error
ssize_t write_number(long num) {
    int num_len = num_digits(num);

    if (num_len > 0) {
        char num_ascii[num_len+1];
        //char *num_ascii = NULL;
        //num_ascii = malloc_safe(num_ascii, (num_len+1));
        //char num_ascii[] = "175990";
        // convert num to an ascii string and store in num_ascii
        num_len = ltoa_dec_buf(num, num_ascii, num_len+1);

        if (num_len > 0) {
            num_len = write(STDOUT_FILENO, &num_ascii, num_len);
        }
    }

    // reassign a result of 0 to -1 to indicate an error
    num_len = (num_len==0) ? -1 : num_len;

    return num_len;
}


// returns the number of chars in num in the given base
int num_digits_gen(long num, int base) {
    int result = -1;

    assert(num >= 0);
    if (num == 0) {
        result = 1;

    } else if (num > 0) {
        // Abort if the calculation of (log10(num) / log10(base))
        // will result in errors from log10 or division by 0.
        assert(log10(num) >= 0 && log10(base) > 0);
        result = (log10(num) / log10(base)) + 1;

    }
    return result;
}


// returns the number of chars in num
int num_digits(long num) {
    return num_digits_gen(num, 10);
}


// converts a long in the specified base to a char buffer
// and returns the number of digits in the number if
// successful. It returns -1 on error, so caller MUST check
// that the result is positive before using.
int ltoa_buf(long num, char *buf, int size, int base) {
    int len = num_digits_gen(num, base);
    if (len > size) {
        len = -1;

    } else if (len > 0) {
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
// Returns the number of digits in the number if
// successful. It returns -1 on error, so caller MUST check
// that the result is positive before using.
int ltoa_dec_buf(long num, char *buf, int size) {
    return ltoa_buf(num, buf, size, 10);
}

