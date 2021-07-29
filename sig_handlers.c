/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/28/2021
**********************/

#include "smallsh.h"


// SIGINT (CTRL-C)
// parent, child background: ignore
// child foreground: terminate itself



// SIGTSTP (CTRL-Z)
// child foreground or background: ignore
// parent: enter or exit foreground-only mode



// SIGCHLD
// for notifying parent that child background is complete
// 
