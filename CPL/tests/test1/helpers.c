#include "helpers.h"

#include <stdio.h>  // For `stderr`

// Shortcuts
#define string char*

const string errors_hints[6] = {
    "E: Memory allocation failed",
    "E: Extra arguments passed",
    "E: Input file with encoded text is not specified",
    "E: Permutations file is not specified",
    "E: Language file with correct text is not specified",
    "E: Can't open specified file",  //
};

// Services
void raise_error(size_t error_code) {
    fprintf(stderr, "%s [%lu]\n", errors_hints[error_code], error_code);
    exit(error_code);
}

void* safe_malloc(size_t count, size_t size) {
    void* allocated = malloc(count * size);
    if (allocated == NULL) {
        raise_error(CANT_ALLOCATE_MEMORY);
    }
    return allocated;
}

void* extend_list(void* list, size_t* length, size_t size) {
    *length *= 2;
    void* reallocated = realloc(list, *length * size);
    if (reallocated == NULL) {
        raise_error(CANT_ALLOCATE_MEMORY);
    }
    return reallocated;
}
