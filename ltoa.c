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


// A non-reentrant, integer only implementation of pow
// so that it can be used in or by a signal handler.
// It implements a subset of the rules shown in the pow man page
// that are relevant for expected inputs in this program
// and may not cover every possible input.
long pow_nonreent(long base, long expon) {
    long result = 1;

    if (base == 1 || expon == 0) {
        result = 1;
    } else if (base == 0 && expon > 0) {
        result = 0;
    } else if (base == 0 && expon < 0 && expon % 2 == 1) {
        result = LONG_MAX;
    } else if (base == 0 && expon < 0) {
        result = +LONG_MAX;
    } else if (expon >= 0) {
        long smallest, largest;

        // One of the previous if/else-if statements should
        // have applied if base is equal to zero.
        // assert to avoid a division by zero
        assert(base != 0);

        smallest = (base > 0) ? LONG_MIN/base : LONG_MAX/base;
        largest  = (base > 0) ? LONG_MAX/base : LONG_MIN/base;

        int i = 0;
        while (i < expon && result > smallest && result < largest) {
            result *= base;
            i++;
        }

        if (result <= smallest) {
            result = LONG_MIN;
        } else if (result >= largest) {
            result = LONG_MAX;
        }

    } else if (expon < 0) {
        int i = 0;
        while (result != 0 && i > expon) {
            result /= base;
            i--;
        }
    }

    return result;
}


// converts a long in the specified base to a char buffer
// and returns the number of digits in the number if
// successful. It returns -1 on error, so caller MUST check
// that the result is positive before using.
int ltoa_buf(long num, char *buf, int size, int base) {
    int start = 0;
    memset(buf, '\0', size);

    if (num < 0) {
        buf[0] = '-';
        num *= -1;
        start++;
    }

    int digit;
    int expon = 0;
    int i = size-1;
    buf[i] = '\0';
    i--;

    for ( ; num > 0 && i >= 0; expon++, i--) {
        digit = num % base;
        buf[i] = 0x30 + digit;
        num /= base;
    }

    // move digits to begin of array and then write \0 over the rest
    memmove(&buf[start], &buf[i+1], expon);
    for (int j = expon; j < size; j++) {
        buf[start+j] = '\0';
    }

    return expon;
}


// decimal long to ascii conversion
// Returns the number of digits in the number if
// successful. It returns -1 on error, so caller MUST check
// that the result is positive before using.
int ltoa_dec_buf(long num, char *buf, int size) {
    return ltoa_buf(num, buf, size, 10);
}

