#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rune_exec.h" // Include the new header
#include "rune_strace_parser.h" // Include the strace parser header
#include "rune_ltrace_parser.h" // Include the ltrace parser header
#include "rune_analyzer.h" // Include the analyzer header
#include "rune_path_finder.h" // Include the path finder header

typedef struct {
    int verbose_mode;
    int static_mode; // This will now imply strace for now, but can be separated later
    int ltrace_mode;
    int valgrind_mode;
    char *target_executable;
    char **target_args;
    int target_argc;
} runescope_config_t;

int main(int argc, char *argv[]) {
    runescope_config_t config = {0}; // Initialize all members to 0/NULL

    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            config.verbose_mode = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--static") == 0) {
            config.static_mode = 1;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--ltrace") == 0) {
            config.ltrace_mode = 1;
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--memcheck") == 0) {
            config.valgrind_mode = 1;
        } else {
            // Found the target executable
            config.target_executable = argv[i];
            config.target_args = &argv[i]; // Point to the start of target executable and its args
            config.target_argc = argc - i;
            break; // Stop parsing runescope's options
        }
    }

    if (config.verbose_mode) {
        printf("Verbose mode enabled.\n");
    }
    if (config.static_mode) {
        printf("Static analysis mode enabled (implies strace for now).\n");
    }
    if (config.ltrace_mode) {
        printf("Ltrace mode enabled.\n");
    }
    if (config.valgrind_mode) {
        printf("Valgrind (Memcheck) mode enabled.\n");
    }

    if (config.target_executable) {
        char *resolved_executable_path = NULL;

        // Resolve the full path of the target executable
        resolved_executable_path = rune_path_finder_find_executable(config.target_executable);

        if (resolved_executable_path == NULL) {
            fprintf(stderr, "runescope: Error: Target executable '%s' not found or not executable.\n", config.target_executable);
            return 1; // Exit with an error code
        }

        // Update argv[0] of the target_args to be the resolved path
        // This is important because execve expects argv[0] to be the path to the executable
        config.target_args[0] = resolved_executable_path;

        printf("Target executable: %s (resolved to %s)\n", config.target_executable, resolved_executable_path);
        printf("Arguments for target executable (%d):\n", config.target_argc);
        for (int j = 0; j < config.target_argc; j++) {
            printf("  argv_target[%d]: %s\n", j, config.target_args[j]);
        }

        // Execute the target program, potentially with strace, ltrace, or valgrind
        printf("\nExecuting target program...\n");
        const char *strace_output_file = "runescope_strace.log";
        const char *ltrace_output_file = "runescope_ltrace.log";
        const char *valgrind_output_file = "runescope_valgrind.log";

        int exit_status = rune_exec_run_target(
            resolved_executable_path, 
            config.target_args, 
            config.static_mode, strace_output_file, 
            config.ltrace_mode, ltrace_output_file, 
            config.valgrind_mode, valgrind_output_file
        );
        if (exit_status != -1) {
            printf("Target program exited with status: %d\n", exit_status);
            if (config.static_mode) {
                printf("Strace output written to: %s\n", strace_output_file);
                rune_analyzer_analyze_strace(strace_output_file);
            }
            if (config.ltrace_mode) {
                printf("Ltrace output written to: %s\n", ltrace_output_file);
                rune_analyzer_analyze_ltrace(ltrace_output_file);
            }
            if (config.valgrind_mode) {
                printf("Valgrind output written to: %s\n", valgrind_output_file);
                // TODO: Add code here to read and parse the valgrind_output_file
            }
        } else {
            fprintf(stderr, "runescope: Error executing target program.\n");
        }
        free(resolved_executable_path); // Free the dynamically allocated path
    } else {
        printf("Usage: %s [-v|--verbose] [-s|--static] [-l|--ltrace] [-m|--memcheck] <executable> [executable_options...]\n", argv[0]);
        printf("Example: %s -v -s -l -m /bin/ls -l -a\n", argv[0]);
    }

    return 0;
}
