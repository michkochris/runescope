#include "rune_ltrace_parser.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int rune_ltrace_parser_parse_file(const char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("runescope: Failed to open ltrace log file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        ltrace_entry_t entry = {0}; // Initialize structure
        char *current_pos = line;
        char *func_name_start = NULL;
        char *args_start = NULL;
        char *return_start = NULL;

        // Skip leading whitespace and PID
        while (*current_pos && (*current_pos == ' ' || (*current_pos >= '0' && *current_pos <= '9'))) {
            current_pos++;
        }
        if (!*current_pos) continue; // Skip if line is empty or only PID

        // Find function name
        func_name_start = current_pos;
        while (*current_pos && *current_pos != '(') {
            current_pos++;
        }
        if (!*current_pos) continue; // Malformed line
        strncpy(entry.function_name, func_name_start, current_pos - func_name_start);
        entry.function_name[current_pos - func_name_start] = '\0';

        // Find arguments
        args_start = current_pos + 1; // Skip '('
        while (*current_pos && *current_pos != ')') {
            current_pos++;
        }
        if (!*current_pos) continue; // Malformed line
        strncpy(entry.args, args_start, current_pos - args_start);
        entry.args[current_pos - args_start] = '\0';

        // Find return value
        current_pos++; // Skip ')'
        while (*current_pos && *current_pos == ' ') {
            current_pos++;
        }
        if (!*current_pos || *current_pos != '=') continue; // Malformed line
        current_pos++; // Skip '='
        while (*current_pos && *current_pos == ' ') {
            current_pos++;
        }
        if (!*current_pos) continue; // Malformed line
        return_start = current_pos;
        while (*current_pos && *current_pos != '\n') {
            current_pos++;
        }
        entry.return_value = atol(return_start);

        // Print parsed information (for testing)
        printf("Parsed Ltrace: PID=%ld, Function=%s, Args='%s', Return=%ld\n",
               entry.pid, entry.function_name, entry.args, entry.return_value);
    }

    fclose(fp);
    return 0;
}
