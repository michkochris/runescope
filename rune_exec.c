/**
 * @brief Valgrind and glibc Debugging Information Issue on Arch Linux WSL
 *
 * When attempting to use Valgrind (specifically the Memcheck tool) on Arch Linux
 * within WSL, a fatal error related to "function redirection" for `memcmp` in
 * `ld-linux-x86-64.so.2` (part of glibc) may occur. This is because Valgrind
 * requires access to unstripped debug symbols for glibc to properly instrument
 * and analyze programs.
 *
 * On Arch Linux, these debug symbols are typically provided by the `glibc-debug`
 * package, which resides in the `debug` and `debug-extra` repositories.
 *
 * Challenges encountered:
 * 1. The `[debug]` and `[debug-extra]` repositories are not enabled by default
 *    in `/etc/pacman.conf`.
 * 2. Even after uncommenting/adding these repositories in `/etc/pacman.conf`,
 *    `pacman -Sy` may fail to synchronize their databases, often with 404 errors
 *    from mirror servers. This indicates that the debug repositories might be
 *    inaccessible or not consistently available from the configured mirrors in WSL.
 *
 * As a result, `glibc-debug` cannot be installed, preventing Valgrind from
 * functioning correctly for memory analysis.
 *
 * Possible future solutions (if this issue persists):
 * - Investigate alternative Arch Linux mirrors for the `debug` repositories.
 * - Manually download and install `glibc-debug` if a reliable source is found.
 * - Consider using a different WSL distribution (e.g., Ubuntu, Debian) where
 *   `libc6-dbg` (the equivalent debug package) is typically easier to install
 *   via `apt`.
 * - Explore Valgrind alternatives if debug symbol installation remains impossible.
 *
 * For the current development, Valgrind's full functionality for memory analysis
 * may be limited or unavailable until this underlying dependency issue is resolved.
 */

#include "rune_exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For fork, execve, _exit
#include <sys/wait.h> // For waitpid
#include <errno.h> // For errno
#include <string.h> // For strlen, strcpy, strcat
#include "rune_path_finder.h" // Include for path finding

// Max arguments for any combination of tools + target program
#define MAX_TOOL_ARGS 256

int rune_exec_run_target(const char *executable_path, char *const argv_target[], 
                         int use_strace, const char *strace_output_path, 
                         int use_ltrace, const char *ltrace_output_path, 
                         int use_valgrind, const char *valgrind_output_path) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("runescope: fork failed");
        return -1;
    } else if (pid == 0) {
        // Child process
        extern char **environ;
        char *exec_argv[MAX_TOOL_ARGS];
        int arg_idx = 0;

        // Determine the primary tool to execute
        const char *primary_tool_name = NULL;
        char *resolved_tool_path = NULL;

        if (use_valgrind) {
            primary_tool_name = "valgrind";
            exec_argv[arg_idx++] = (char *)primary_tool_name;
            exec_argv[arg_idx++] = "--tool=memcheck"; // Default to memcheck
            
            // Construct the --log-file argument correctly
            char *log_file_arg = (char *)malloc(strlen("--log-file=") + strlen(valgrind_output_path) + 1);
            if (log_file_arg == NULL) {
                perror("runescope: malloc failed for valgrind log file arg");
                _exit(EXIT_FAILURE);
            }
            strcpy(log_file_arg, "--log-file=");
            strcat(log_file_arg, valgrind_output_path);
            exec_argv[arg_idx++] = log_file_arg;

            exec_argv[arg_idx++] = "--leak-check=full";
            exec_argv[arg_idx++] = "--show-leak-kinds=all";
            exec_argv[arg_idx++] = "--track-origins=yes";

            // Add the -- separator for valgrind to indicate end of its options
            exec_argv[arg_idx++] = "--";
        }

        if (use_ltrace) {
            // If valgrind is also used, ltrace is an argument to valgrind
            if (use_valgrind) {
                exec_argv[arg_idx++] = "ltrace"; // Pass 'ltrace' as an argument to valgrind
            } else if (!primary_tool_name) { // If ltrace is the primary tool
                primary_tool_name = "ltrace";
                exec_argv[arg_idx++] = (char *)primary_tool_name;
            }
            exec_argv[arg_idx++] = "-o";
            exec_argv[arg_idx++] = (char *)ltrace_output_path;
            exec_argv[arg_idx++] = "-f"; // Trace child processes
        }

        if (use_strace) {
            // If valgrind or ltrace is also used, strace is an argument to the preceding tool
            if (use_valgrind || use_ltrace) {
                exec_argv[arg_idx++] = "strace"; // Pass 'strace' as an argument to valgrind/ltrace
            } else if (!primary_tool_name) { // If strace is the primary tool
                primary_tool_name = "strace";
                exec_argv[arg_idx++] = (char *)primary_tool_name;
            }
            exec_argv[arg_idx++] = "-o";
            exec_argv[arg_idx++] = (char *)strace_output_path;
            exec_argv[arg_idx++] = "-f"; // Trace child processes
        }

        // Resolve the path of the primary tool
        resolved_tool_path = rune_path_finder_find_executable(primary_tool_name);
        if (resolved_tool_path == NULL) {
            fprintf(stderr, "runescope: Error: Tool '%s' not found in PATH or not executable.\n", primary_tool_name);
            _exit(EXIT_FAILURE);
        }

        // The first argument to execve must be the path to the executable itself
        // This is already handled by setting exec_argv[0] to resolved_tool_path

        // Add the target executable and its arguments
        exec_argv[arg_idx++] = (char *)executable_path;
        for (int i = 1; argv_target[i] != NULL && arg_idx < MAX_TOOL_ARGS - 1; i++) {
            exec_argv[arg_idx++] = argv_target[i];
        }
        exec_argv[arg_idx] = NULL; // Null-terminate the argument list

        execve(resolved_tool_path, exec_argv, environ);
        perror("runescope: execve tool failed");
        free(resolved_tool_path); // Free the dynamically allocated path
        // Free the valgrind log_file_arg if it was allocated
        if (use_valgrind) {
            // The log_file_arg is at index 2 if valgrind is the primary tool
            // and we added --tool=memcheck at index 1.
            // This is fragile, a better way is to store the pointer.
            free(exec_argv[2]); 
        }
        _exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            // Defensive programming: Handle waitpid failure
            perror("runescope: waitpid failed");
            return -1; // Indicate an error in runescope itself
        }

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status); // Return the exit status of the child
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "runescope: Target program terminated by signal %d\n", WTERMSIG(status));
            return -1; // Or a specific error code for signal termination
        } else {
            // Defensive programming: Handle other unexpected termination scenarios
            fprintf(stderr, "runescope: Target program terminated abnormally.\n");
            return -1;
        }
    }
}
