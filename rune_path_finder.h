#ifndef RUNE_PATH_FINDER_H
#define RUNE_PATH_FINDER_H

/**
 * @brief Searches for an executable in the system's PATH.
 *
 * This function takes an executable name (e.g., "ls", "cat") and searches
 * through the directories specified in the PATH environment variable.
 * It returns the full, absolute path to the executable if found and executable.
 *
 * @param executable_name The name of the executable to find (e.g., "ls").
 * @return A dynamically allocated string containing the full path to the executable
 *         if found, or NULL if not found or an error occurred. The caller is
 *         responsible for freeing the returned string.
 */
char *rune_path_finder_find_executable(const char *executable_name);

#endif // RUNE_PATH_FINDER_H
