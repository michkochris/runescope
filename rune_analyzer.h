#ifndef RUNE_ANALYZER_H
#define RUNE_ANALYZER_H

#include "rune_strace_parser.h"
#include "rune_ltrace_parser.h"

/**
 * @brief Analyzes parsed strace and ltrace data for various insights.
 *
 * This module will contain functions to process the raw parsed system call
 * and library call data to identify patterns related to memory management,
 * performance, security, and defensive programming.
 */

/**
 * @brief Performs a basic analysis of strace data.
 *
 * This function will read the strace log file, parse it, and provide a summary
 * of system call activities, such as file operations, process management, etc.
 * It will also highlight potential areas of interest for security or performance.
 *
 * @param strace_log_path The path to the strace log file.
 * @return 0 on success, -1 on failure.
 */
int rune_analyzer_analyze_strace(const char *strace_log_path);

/**
 * @brief Performs a basic analysis of ltrace data.
 *
 * This function will read the ltrace log file, parse it, and provide a summary
 * of library call activities, focusing on memory allocation/deallocation,
 * string manipulations, and other high-level library interactions.
 *
 * @param ltrace_log_path The path to the ltrace log file.
 * @return 0 on success, -1 on failure.
 */
int rune_analyzer_analyze_ltrace(const char *ltrace_log_path);

#endif // RUNE_ANALYZER_H
