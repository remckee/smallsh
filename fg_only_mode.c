/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/31/2021
**********************/

#include "smallsh.h"


// Isolating this "file scope" variable in its own file
// so that other functions besides toggle_fg_only() and
// get_fg_only() cannot access it directly.
static bool fg_only = false;


// returns value of fg_only variable
bool get_fg_only() {
    return fg_only;
}


// changes value of fg_only to the opposite value
void toggle_fg_only() {
    fg_only = !fg_only;
}

