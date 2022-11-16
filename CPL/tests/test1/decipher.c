#include "decipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

// Shortcuts
#define string char*
#define loop_i(repeats) for (size_t i = 0; i < repeats; ++i)

// Services
string apply_permutation(string base, int* permutation) {
    size_t length = strlen(base);
    // Iterator for the mutated string
    int m = 0;
    string mutated = safe_malloc(length, sizeof(char));
    loop_i(length) {
        mutated[m++] = base[permutation[i]];  //
    }
    // Put correct end of string
    mutated[m] = '\0';
    return mutated;
}
