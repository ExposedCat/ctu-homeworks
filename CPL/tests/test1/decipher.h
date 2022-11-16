#ifndef DECIPHER_H
#define DECIPHER_H
#include <stdlib.h>  // For `size_t`

// Shortcuts
#define string char*

// Services
string apply_permutation(string base, int* permutation);

#endif
