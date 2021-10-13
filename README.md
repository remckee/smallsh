# smallsh

a small shell implemented in C

To compile, run:

    chmod u+x compile.sh
    ./compile.sh
    
Program Functionality
1. The Command Prompt

    Use the colon `:` symbol as a prompt for each command line.

    The general syntax of a command line is:

        command [arg1 arg2 ...] [< input_file] [> output_file] [&]

    where items in square brackets are optional.
    
2. Comments & Blank Lines

    Any line that begins with the `#` character is a comment line and should be ignored.
    
    A blank line (one without any commands) also does nothing.
    
3. Expansion of Variable `$$`

    Expands any instance of `$$` in a command into the process ID of the smallsh itself.

4. Built-in Commands

    Supports three built-in commands: `exit`, `cd`, and `status`. All others are simply passed on to a member of the exec() family of functions.

    `exit`
    
    The `exit` command exits smallsh. It takes no arguments.

    `cd`
    
    The `cd` command changes the working directory of smallsh.
    
    With no arguments, it changes to the directory specified in the HOME environment variable.
    
    This command can also take one argument: the path of a directory to change to. Supports both absolute and relative paths.

    `status`
    
    The `status` command prints out either the exit status or the terminating signal of the last foreground process ran by smallsh. If this command is run before any foreground command is run, then it should simply return the exit status 0. The three built-in shell commands do not count as foreground processes for the purposes of this built-in command - i.e., status should ignore built-in commands.

5. Executing Other Commands

    Whenever a non-built in command is received, the parent (i.e., smallsh) will fork off a child. The child will use a function from the exec() family of functions to run the command.
    
    If a command fails because the shell could not find the command to run, then the shell will print an error message and set the exit status to 1.

6. Input & Output Redirection

    Both stdin and stdout for a command can be redirected at the same time.
    
    In an input or output file cannot be opened, prints an error message and sets the exit status to 1 (but doesn't exit shell).
