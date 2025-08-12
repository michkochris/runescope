#ifndef RUNE_EXEC_H
#define RUNE_EXEC_H

/**
 * @brief Executes a target program with its arguments, optionally using strace, ltrace, and valgrind.
 *
 * This function forks a new process and uses execve to run the target executable.
 * It can chain strace, ltrace, and valgrind based on the provided flags.
 * It includes basic error checking for fork and execve.
 *
 * @param executable_path The absolute path to the target executable.
 * @param argv_target An array of strings representing the arguments for the target executable,
 *                    starting with the executable's name itself (argv[0]).
 * @param use_strace If true, the target program will be run under strace.
 * @param strace_output_path If use_strace is true, the path to the file where strace output will be written.
 * @param use_ltrace If true, the target program will be run under ltrace.
 * @param ltrace_output_path If use_ltrace is true, the path to the file where ltrace output will be written.
 * @param use_valgrind If true, the target program will be run under valgrind (memcheck).
 * @param valgrind_output_path If use_valgrind is true, the path to the file where valgrind output will be written.
 * @return The exit status of the executed program, or -1 if an error occurred in runescope itself.
 */
int rune_exec_run_target(const char *executable_path, char *const argv_target[], 
                         int use_strace, const char *strace_output_path, 
                         int use_ltrace, const char *ltrace_output_path, 
                         int use_valgrind, const char *valgrind_output_path);

#endif // RUNE_EXEC_H
