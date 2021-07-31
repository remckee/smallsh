#include "smallsh.h"

/* int get_s(int c) { */
/*     static int s = 0; */

/*     return s; */
/* } */


/* int main (int argc, char *argv[]) { */
/*     long num = pow_nonreent(atol(argv[1]), atol(argv[2])); */
/*     printf("num: %ld\n", num); */
/*     int size = 21; // 9223372036854775807 has 19 chars + 2 for sign and \0 */
/*     //char *result; */

/*   //  if (size > 0) { */
/*         char pid_ascii[size]; */

/*     //int s = ltoa_dec_buf(num, pid_ascii, size); */
/*     ltoa_dec_buf(num, pid_ascii, size); */
/*     printf("len num:, ascii: %s\n", pid_ascii); */

/*       write_number (num); */
/*     pid_t pid = getpid(); */
/*     char *str = "cd$$e q$$x"; */
/*     int exp_count = 0; */
/*     str = expand_vars(str, pid, &exp_count); */

/*     printf("%d\n", pid); */
/*     printf("new: %s %d %ld \n", str, exp_count, strlen(str)); */

/*     if (exp_count > 0) { */
/*         free_safe(str); */
/*     } */

/* //bool get_cmd_test(struct cmd_line *cmd_parts, char *quit) */
/*         //struct cmd_line *cmd_parts = malloc(sizeof(struct cmd_line)); */
/*         // = malloc(sizeof(struct cmd_line)); */
/*         cmd_parts.cmd = NULL; */
/*         cmd_parts.argsc = 0; */
/*         cmd_parts.input_file = NULL; */
/*         cmd_parts.output_file = NULL; */
/*         cmd_parts.background = false; */
/*         char quit = ' '; */
/*         bool skip; */
/*         struct cmd_line *cmd_parts = get_cmd_test(&quit, &skip); */
/*        // struct cmd_line *cmd_parts = get_cmd(&quit); */

/*         if (!skip) { */

/*             assert(cmd_parts->cmd); */
/*         } */

/*         if (cmd_parts->cmd) { */
/*             printf("command from main: %s\n", cmd_parts->cmd); */
/*             fflush(NULL); */
/*         } */
/*         free_safe(cmd_parts); */
/* //    return 0; */
/* } */
