#include "rune_analyzer.h"
#include <stdio.h>

int rune_analyzer_analyze_strace(const char *strace_log_path) {
    printf("\n--- Analyzing Strace Data ---\n");
    // For now, just re-parse and print. Actual analysis logic will go here.
    return rune_strace_parser_parse_file(strace_log_path);
}

int rune_analyzer_analyze_ltrace(const char *ltrace_log_path) {
    printf("\n--- Analyzing Ltrace Data ---\n");
    // For now, just re-parse and print. Actual analysis logic will go here.
    return rune_ltrace_parser_parse_file(ltrace_log_path);
}
