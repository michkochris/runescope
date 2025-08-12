#ifndef RUNE_STRACE_PARSER_H
#define RUNE_STRACE_PARSER_H

#include <stdio.h>

// Structure to hold parsed strace entry data
typedef struct {
    long pid;
    char syscall_name[64]; // Max length for syscall name
    char args[512];        // Arguments as a raw string for now
    long return_value;
    char error_str[128];   // Error string if present (e.g., ENOENT)
    int has_error;
} strace_entry_t;

/**
 * @brief Parses a strace log file and prints the extracted information.
 *
 * This function reads the specified strace log file line by line,
 * attempts to parse each line into a strace_entry_t structure,
 * and prints the parsed details.
 * In future iterations, this function will store the parsed data
 * in a more robust data structure for further analysis.
 *
 * @param file_path The path to the strace log file.
 * @return 0 on success, -1 on failure (e.g., file not found).
 */
int rune_strace_parser_parse_file(const char *file_path);

#endif // RUNE_STRACE_PARSER_H
