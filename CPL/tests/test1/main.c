#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decipher.h"
#include "helpers.h"

// Shortcuts
#define string char*
#define File FILE*
#define forever while(1)

// Helpers
File open_file(string path);
string read_line(File file, size_t length);
void reopen_file(File* file, string path);

// Services
void parse_arguments(                    //
    int count, string arguments[],       //
    File* input, File* perm, File* lang  //
);
void close_fileset(File input, File perm, File lang);
int* read_permutation(File perm, size_t* perm_length);

// MAIN
int main(int argc, string argv[]) {
    File input = NULL;
    File perm = NULL;
    File lang = NULL;
    parse_arguments(argc, argv, &input, &perm, &lang);

    size_t length = 0;
    int* permutation = NULL;
    forever {
        permutation = read_permutation(perm, &length);
        if (!permutation) {
            break;
        }
        for (size_t i = 0; i < length; ++i) {
            printf("%s%d", i ? " " : "", permutation[i]);
        }
        printf(":\n");
        string word = "None";
        forever {
            word = read_line(input, length);
            if (!word) {
                break;
            }
            string mutated = apply_permutation(word, permutation);
            printf("«%s» → «%s»\n", word, mutated);
            free(mutated);
            free(word);
        }

        free(permutation);
        reopen_file(&input, argv[1]);
    }

    close_fileset(input, perm, lang);
    return 0;
}

// Helpers
File open_file(string path) {
    File file = fopen(path, "r");
    if (file == NULL) {
        raise_error(CANT_OPEN_FILE);
    }
    return file;
}

string read_line(File file, size_t length) {
    string line = safe_malloc(length, sizeof(char));
    int result = fscanf(file, "%[^\n]%*c", line);
    return result == 1 ? line : NULL;
}

void reopen_file(File* file, string path) {
    fclose(*file);
    *file = open_file(path);
}

// bool word_is_correct(File*)

// Services
void close_fileset(File input, File perm, File lang) {
    fclose(input);
    fclose(perm);
    fclose(lang);
}

void parse_arguments(                    //
    int count, string arguments[],       //
    File* input, File* perm, File* lang  //
) {
    // Note: There will always be one more (first) argument - program, use
    // indices and count from 1 Raise error on invalid arguments passed
    switch (count) {
        case 1: {
            raise_error(NO_INPUT_FILE);
            break;
        }
        case 2: {
            raise_error(NO_PERM_FILE);
            break;
        }
        case 3: {
            raise_error(NO_LANG_FILE);
            break;
        }
        case 4: {
            // OK
            break;
        }
        default: {
            raise_error(EXTRA_ARGUMENTS);
        }
    }
    *input = open_file(arguments[1]);
    *perm = open_file(arguments[2]);
    *lang = open_file(arguments[3]);
}

int* read_permutation(File perm, size_t* perm_length) {
    size_t length = 1;
    // Index for the permutation
    size_t i = 0;
    int* permutation = safe_malloc(length, sizeof(int));

    bool reached_end = false;
    forever {  // Read permutation
        // Next permutation index
        int index = 0;
        // Next read character
        char c = '\n';
        bool reached_perm_end = false;
        forever {  // Read a number from a line
            int result = fscanf(perm, "%c", &c);
            bool is_dash = c == '-';
            reached_end = result == EOF;
            reached_perm_end = reached_end || is_dash;
            if (is_dash) {
                fgetc(perm);  // Dash
                fgetc(perm);  // Dash
                fgetc(perm);  // Newline
            }
            if (reached_end || reached_perm_end || c == '\n') {
                index /= 10;
                break;
            }
            // Char-zero subtraction shifts ASCII code of c to the one
            // corresponding to the number at c
            index += c - '0';
            index *= 10;
        }
        if (reached_perm_end) {
            break;
        } else {
            // Allocate more memory for the array if needed
            if (i >= length) {
                permutation = extend_list(  //
                    permutation,            //
                    &length, sizeof(int)    //
                );
            }
            permutation[i++] = index;
        }
    }
    // Free extra memory from array if needed
    if (i != length) {
        permutation = realloc(permutation, i * sizeof(int));
    }
    *perm_length = i;

    if (reached_end) {
        return NULL;
    }
    return permutation;
}
