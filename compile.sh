#!/bin/bash
files="expand.c built_in.c sig_handlers.c main.c error_warn.c run.c process_args.c fg_only_mode.c redirection.c"
gcc -std=c11 -Wall -Werror -g3 -O0 -o smallsh $files -D_POSIX_C_SOURCE=200809L -lm
