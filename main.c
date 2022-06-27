#include "smallsh.h"

int main (int argc, char *argv[]) {
    int status_val = SUCCESS;       // status or signal of the last fg process
    int status_type = CLD_EXITED;   // what caused the last fg process to end
    const pid_t SH_PID = getpid();  // pid of sh shell
    pid_t bg_procs[MAX_PROCS];      // array of pids of all bg processes

    int size = 21;  // Max number of characters for a long:
                    // 9223372036854775807 has 19 chars + 2 for sign and \0

    char pid_ascii[size];   // used to store the ascii characters of pid

    // convert shell pid to an ascii string and store in pid_ascii
    size = ltoa_dec_buf(SH_PID, pid_ascii, size);

    // exit with error if pid conversion failed
    exit_if_error (size <= 0, "ltoa error");

    // Initialize parent signal handlers
    init_parent_sig_handlers();

    /*
     * Initialize all elements in bg_procs array to -1. The value -1 is used
     * to indicate which elements in the array haven't been used yet. Then,
     * when iterating through the array, we can stop when we reach an element
     * whose value is -1, rather than going through the entire array.
     */
    init_procs(bg_procs);

    while (1) {
        bool skip;          // Set to true if we should skip processing the
                            // rest of the command line, for various reasons.

        /*
         * Get command line from user.
         * get_cmd sets skip to false if command was successfully parsed
         * into cmd_parts struct and true otherwise
         */
        struct cmd_line *cmd_parts = get_cmd(&skip, pid_ascii);

        /*
         * If a valid non-comment command was successfully parsed,
         * route command to built-in, fg external, or bg external.
         */
        if (!skip) {
            assert(cmd_parts->cmd);
            if (is_built_in(cmd_parts->cmd)) {
                run_built_in(cmd_parts, status_val, status_type, bg_procs);

            } else {
                /*
                 * get_fg_only() returns the file-scoped variable in
                 * fg_only_mode.c that indicates whether or not we are in
                 * fg only mode.
                 */
                if (get_fg_only() || !(cmd_parts->background)) {
                    // run external command in the foreground
                    run_external_fg(cmd_parts, &status_val, &status_type);

                } else {
                    // reun external command in the background
                    run_external_bg(cmd_parts, BG_DEFAULT, BG_DEFAULT, bg_procs);
                }
            }
        }

        // free memory that may have been allocated when parsing command
        free_cmd(cmd_parts);

        /*
         * check if any background processes have terminated, and report the
         * status for any that have terminated
         */
        check_procs(bg_procs);
    }

    return 0;
}
