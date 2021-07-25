#*********************
# Name: Rebecca Mckeever
# Course: CS 344
# Assignment 3
# Last edited: 07/23/2021
#*********************

#!/bin/bash

gcc -std=c11 -Wall -Werror -g3 -O0 -o smallsh *.c -D_POSIX_C_SOURCE=200809L
