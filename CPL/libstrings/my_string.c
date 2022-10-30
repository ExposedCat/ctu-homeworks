#include "my_string.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long strspn(const char* dest, const char* src) {
    unsigned long matches = 0;
    for (int i = 0;; ++i) {
        if (dest[i] == '\0') {
            break;
        }
        bool unknown_char = true;
        for (int j = 0;; ++j) {
            if (src[j] == '\0') {
                break;
            }
            if (dest[i] == src[j]) {
                matches++;
                unknown_char = false;
                break;
            }
        }
        if (unknown_char) {
            break;
        }
    }
    return matches;
}

// • Static pointer used to remember previous token
static char* token = NULL;
// • This is really weird, I believe that with a different signature it can
// be implemented in a better and less mystical way
char* strtok(char* str, const char* delim) {
    // On subsequent calls we start from the pointer stored previously
    if (str == NULL) {
        if (token == NULL) {
            return NULL;
        }
        str = token;
    }
    // Search for a character that is not contained in the list of delimiters
    // It will be a beginning of the token
    char* next_token = NULL;
    for (int i = 0;; ++i) {
        if (next_token != NULL || str[i] == '\0') {
            break;
        }
        for (int j = 0;; ++j) {
            if (delim[j] == '\0') {
                break;
            }
            if (str[i] == delim[j]) {
                next_token = NULL;
                break;
            }
            next_token = &str[i];
        }
    }
    if (next_token == NULL) {
        return NULL;
    }
    // Search for a character that IS contained in the list of delimiters
    char* next_delim = NULL;
    for (int i = 0;; ++i) {
        if (next_delim != NULL || str[i] == '\0') {
            break;
        }
        for (int j = 0;; ++j) {
            if (delim[j] == '\0') {
                break;
            }
            if (str[i] == delim[j]) {
                next_delim = &str[i];
                break;
            }
        }
    }
    if (next_delim == NULL) {
        token = NULL;
    } else {
        *next_delim = '\0';
        // Shift by 1 to skip first `\0` at all the subsequent calls
        token = next_delim + 1;
    }
    return next_token;
}

// !WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!
// • Changed to `**` since we have to write a pointer in case of re-allocation.
// Pass `dest` with &
void strcat(char** dest, const char* src) {
    // • It's impossible (or idk the way) to determine allocated memory size
    // (there can be more than one \0), so I will reallocate always
    int length = 0;
    int add_dest = 1;
    int add_src = 1;
    for (int i = 0;; ++i) {
        if (add_dest && (*dest)[i] == '\0') {
            add_dest = 0;
        }
        if (add_src && src[i] == '\0') {
            add_src = 0;
        }
        int add = add_src + add_dest;
        if (!add) {
            break;
        }
        length += add_src + add_dest;
    }
    *dest = realloc(*dest, sizeof(char) * (length + 1));
    char* start = NULL;
    for (int i = 0;; ++i) {
        if ((*dest)[i] == '\0') {
            start = &(*dest)[i];
            break;
        }
    }
    for (int i = 0;; ++i) {
        *(start + i) = src[i];
        if (src[i] == '\0') {
            break;
        }
    }
}

// • Changed to `const` since writing `const` pointers to non-const ones
// considered as a bad practice (discards qualifiers)
// !WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!WARNING!
// • Changed to `**` since we have to write a pointer, not a value. Pass `begin`
// with &
unsigned long strstr_match(                                  //
    const char* str, const char* substr, const char** begin  //
) {
    unsigned long length = 0;
    for (int i = 0;; ++i) {
        bool exit = true;
        *begin = &str[i];
        bool group = false;
        bool char_found = false;
        for (int j = 0;; ++j) {
            if (substr[j] == '\0' || str[i + j] == '\0') {
                if (str[i] == '\0') {
                    exit = true;
                }
                break;
            }
            if (substr[j] == '[') {
                group = true;
                char_found = false;
                i -= 1;
                continue;
            }
            if (substr[j] == ']') {
                group = false;
                if (!char_found) {
                    length = 0;
                    exit = true;
                    break;
                }
                i -= 1;
                continue;
            }
            if (str[i + j] != substr[j]) {
                if (group) {
                    i -= 1;
                    continue;
                }
                length = 0;
                exit = false;
                break;
            }
            if (!group || !char_found) {
                length += 1;
            }
            if (group) {
                char_found = true;
            }
        }
        if (exit) {
            break;
        }
    }
    if (!length) {
        *begin = NULL;
    }
    return length;
}
