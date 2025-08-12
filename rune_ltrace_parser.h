#ifndef RUNE_LTRACE_PARSER_H
#define RUNE_LTRACE_PARSER_H

#include <stdio.h>

// Structure to hold parsed ltrace entry data
typedef struct {
    long pid;
    char function_name[128]; // Max length for function name
    char args[512];          // Arguments as a raw string
    long return_value;
} ltrace_entry_t;

/**
 * @brief Parses an ltrace log file and prints the extracted information.
 *
 * This function reads the specified ltrace log file line by line,
 * attempts to parse each line into an ltrace_entry_t structure,
 * and prints the parsed details.
 * In future iterations, this function will store the parsed data
 * in a more robust data structure for further analysis.
 *
 * @param file_path The path to the ltrace log file.
 * @return 0 on success, -1 on failure (e.g., file not found).
 */
int rune_ltrace_parser_parse_file(const char *file_path);

#endif // RUNE_LTRACE_PARSER_H
