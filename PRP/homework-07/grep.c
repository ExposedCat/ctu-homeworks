#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Constants
#define E_NUM 2  // Number of possible errors
#define FOUND 0
#define NOT_FOUND 1

// Types
#define string char*
#define File FILE*
enum errors { UNDEFINED_PATTERN, CANT_OPEN_FILE };
typedef struct SizedList {
    int size;
    string* body;
} SizedList;

// Shortcuts
#define List SizedList*
#define forever while (1)
#define mem(type, size) sizeof(type) * (size)

const string error_hints[E_NUM] = {
    //
    "Search pattern is not specified",    //
    "File to search at is not specified"  //
};

string regex_flag = "-E";
string color_flag = "--color=always";

const int error_codes[E_NUM] = {2, 3};

// Helpers
void raise_error(int code);
File open_file(string path);
bool contains(int s_length, string source, int sub_length, string substring);
bool equals(string source1, string source2);
int get_string_length(string source);
void add_char(int* length, string* source, char character);
string read_line(File file, int* length, bool* end);
void parse_args(                 //
    int count, string* args,     //
    bool* color, bool* regex,    //
    File* file, string* pattern  //
);

int main(int argc, string* argv) {
    if (argc == 1) {
        raise_error(UNDEFINED_PATTERN);
    }
    bool color, regex;
    File file;
    string pattern;
    parse_args(argc, argv, &color, &regex, &file, &pattern);
    int pattern_length = get_string_length(pattern);

    int exit_code = NOT_FOUND;

    forever {
        int str_length = 0;
        bool exit;
        string read = read_line(file, &str_length, &exit);
        if (exit) {
            break;
        }
        bool found = contains(str_length, read, pattern_length, pattern);
        if (found) {
            exit_code = FOUND;
            // TODO: Implement color
            printf("%s\n", read);
        }
        free(read);
    }

    fclose(file);
    return exit_code;
}

void parse_args(                 //
    int count, string* args,     //
    bool* color, bool* regex,    //
    File* file, string* pattern  //
) {
    *color = false;
    *regex = false;
    for (int i = 0; i < count; ++i) {
        if (equals(args[i], color_flag)) {
            *color = true;
        } else if (equals(args[i], regex_flag)) {
            *regex = true;
        }
        if (*color && *regex) {
            break;
        }
    }
    // By default, take pattern from 1st argument, read stdin
    *pattern = args[1];
    *file = stdin;
    if (count == 2) {
        // Finish processing arguments if there is no more
        return;
    }
    switch (count) {
        case 3: {
            if (*color || *regex) {
                // If option, take pattern from 2nd argument, read stdin
                *pattern = args[2];
            } else {
                // If no options, take pattern from 1st argument, read file from
                // 2nd argument
                *file = open_file(args[2]);
            }
            break;
        }
        case 4: {
            if (*color && *regex) {
                // If both options, take pattern from 3rd argument, read stdin
                *pattern = args[3];
            } else {
                // If not both options, take pattern from 2nd argument, read
                // file from 3rd argument
                *pattern = args[2];
                *file = open_file(args[3]);
            }
            break;
        }
        default: {
            // If more options, take pattern from 3nd argument, read file from
            // 4th argument. Ignore the rest
            *pattern = args[3];
            *file = open_file(args[4]);
        }
    }
}

string read_line(File file, int* length, bool* end) {
    string read = malloc(0);
    forever {
        char character;
        int scanned = fscanf(file, "%c", &character);
        if (                                          //
            character == '\n' || character == EOF ||  //
            !scanned || scanned == -1                 //
        ) {
            *end = scanned == -1;
            return read;
        }
        add_char(length, &read, character);
    }
}

void add_char(int* length, string* source, char character) {
    (*source)[*length] = character;
    *length += 1;
    *source = realloc(*source, mem(string, *length));
    (*source)[*length] = '\0';
}

int get_string_length(string source) {
    int length = 0;
    forever {
        if (source[length] == '\0') {
            return length;
        } else {
            length += 1;
        }
    }
}

bool equals(string source1, string source2) {
    int length1 = get_string_length(source1);
    int length2 = get_string_length(source2);
    if (length1 != length2) {
        return false;
    }
    for (int i = 0; i < length1; ++i) {
        if (source1[i] != source2[i]) {
            return false;
        }
    }
    return true;
}

bool contains(int s_length, string source, int sub_length, string substring) {
    if (s_length < sub_length) {
        return false;
    }
    for (int offset = 0; offset <= s_length - sub_length; ++offset) {
        bool shift = false;
        // TODO: Implement regex
        for (int i = 0; i < sub_length; ++i) {
            if (source[offset + i] != substring[i]) {
                shift = true;
                break;
            }
        }
        if (shift) {
            continue;
        }
        return true;
    }
    return false;
}

File open_file(string path) {
    File file = fopen(path, "r");
    if (file == NULL) {
        raise_error(CANT_OPEN_FILE);
    }
    return file;
}

void raise_error(int code) {
    fprintf(stderr, "Error: %s\n", error_hints[code]);
    exit(error_codes[code]);
}
