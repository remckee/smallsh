#*********************
# Name: Rebecca Mckeever
# Course: CS 344
# Assignment 3
# Last edited: 07/28/2021
#*********************

#!/bin/bash
#cd.c exit.c functions.c main.c process_args.c status.c
#gcc -std=c11 -Wall -Werror -g3 -O0 -o smallsh *.c -D_POSIX_C_SOURCE=200809L -lm


gcc -std=c11 -Wall -Werror -g3 -O0 -o test test.c ltoa.c -D_POSIX_C_SOURCE=200809L -lm