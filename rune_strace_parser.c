#include "rune_strace_parser.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int rune_strace_parser_parse_file(const char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("runescope: Failed to open strace log file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        strace_entry_t entry = {0}; // Initialize structure
        char *syscall_start = NULL;
        char *args_start = NULL;
        char *return_start = NULL;
        char *error_start = NULL;

        // Attempt to parse lines like: PID SYSCALL_NAME(ARGS) = RETURN_VALUE ERROR_STRING
        // Or: PID SYSCALL_NAME(ARGS) = RETURN_VALUE

        // Find PID
        entry.pid = atol(line);
        char *current_pos = line;
        while (*current_pos && (*current_pos == ' ' || (*current_pos >= '0' && *current_pos <= '9'))) {
            current_pos++;
        }
        if (!*current_pos) continue; // Skip if only PID is present or line is malformed

        // Find syscall name
        syscall_start = current_pos;
        while (*current_pos && *current_pos != '(') {
            current_pos++;
        }
        if (!*current_pos) continue;
        strncpy(entry.syscall_name, syscall_start, current_pos - syscall_start);
        entry.syscall_name[current_pos - syscall_start] = '\0';

        // Find arguments
        args_start = current_pos + 1; // Skip '(' 
        while (*current_pos && *current_pos != ')') {
            current_pos++;
        }
        if (!*current_pos) continue;
        strncpy(entry.args, args_start, current_pos - args_start);
        entry.args[current_pos - args_start] = '\0';

        // Find return value
        current_pos++; // Skip ')'
        while (*current_pos && *current_pos == ' ') {
            current_pos++;
        }
        if (!*current_pos || *current_pos != '=') continue;
        current_pos++; // Skip '='
        while (*current_pos && *current_pos == ' ') {
            current_pos++;
        }
        if (!*current_pos) continue;
        return_start = current_pos;
        while (*current_pos && *current_pos != ' ' && *current_pos != '\n') {
            current_pos++;
        }
        entry.return_value = atol(return_start);

        // Check for error string
        if (*current_pos == ' ') {
            error_start = current_pos + 1;
            while (*current_pos && *current_pos != '\n') {
                current_pos++;
            }
            strncpy(entry.error_str, error_start, current_pos - error_start);
            entry.error_str[current_pos - error_start] = '\0';
            entry.has_error = 1;
        }

        // Print parsed information (for testing)
        printf("Parsed: PID=%ld, Syscall=%s, Args='%s', Return=%ld",
               entry.pid, entry.syscall_name, entry.args, entry.return_value);
        if (entry.has_error) {
            printf(", Error='%s'", entry.error_str);
        }
        printf("\n");
    }

    fclose(fp);
    return 0;
}
