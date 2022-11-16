#ifndef HELPERS_H
#define HELPERS_H
#include <stdlib.h>  // For `size_t`

// Services
enum Errors {
    CANT_ALLOCATE_MEMORY,
    EXTRA_ARGUMENTS,
    NO_INPUT_FILE,
    NO_PERM_FILE,
    NO_LANG_FILE,
    CANT_OPEN_FILE
};

void raise_error(size_t error_code);

void* safe_malloc(size_t count, size_t size);

void* extend_list(void* list, size_t* length, size_t size);

#endif
