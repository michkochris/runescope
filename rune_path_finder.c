#define _POSIX_C_SOURCE 200809L // For strdup
#include "rune_path_finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For access

char *rune_path_finder_find_executable(const char *executable_name) {
    if (executable_name == NULL || strlen(executable_name) == 0) {
        return NULL;
    }

    // If the executable_name contains a slash, it's an absolute or relative path
    // and we don't need to search PATH.
    if (strchr(executable_name, '/') != NULL) {
        if (access(executable_name, X_OK) == 0) {
            return strdup(executable_name); // Return a dynamically allocated copy
        }
        return NULL;
    }

    char *path_env = getenv("PATH");
    if (path_env == NULL) {
        fprintf(stderr, "runescope: PATH environment variable not set.\n");
        return NULL;
    }

    // Duplicate PATH because strtok modifies the string
    char *path_copy = strdup(path_env);
    if (path_copy == NULL) {
        perror("runescope: strdup failed");
        return NULL;
    }

    char *dir = strtok(path_copy, ":");
    char full_path[1024]; // Buffer for full path

    while (dir != NULL) {
        // Construct full path: directory + / + executable_name
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, executable_name);

        // Check if the file exists and is executable
        if (access(full_path, X_OK) == 0) {
            free(path_copy); // Free the duplicated PATH
            return strdup(full_path); // Return a dynamically allocated copy
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy); // Free the duplicated PATH
    return NULL; // Executable not found in PATH
}
