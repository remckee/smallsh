/*********************
Name: Rebecca Mckeever
Course: CS 344
Assignment 3
Last edited: 07/23/2021
**********************/

#include "smallsh.h"


//int main(int argc, char *argv[]){
int mycd(int argc, char *argv[]) {
    int result;

    if (argc >= 2) {
        result = chdir(argv[1]);
    } else if (argc == 1) {
        result = chdir(getenv("HOME"));
    }

    //exit(EXIT_SUCCESS);
    return result;
}
