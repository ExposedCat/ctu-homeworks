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
    int* body;
} SizedList;

// Shortcuts
#define List SizedList*
#define forever while (1)
#define mem(type, size) sizeof(type) * (size)

const string error_hints[E_NUM] = {
    //
    "Search pattern is not specified",            //
    "Can't open file: no such file \
    or program has not enough rights to open it"  //
};

string REGEX_FLAG = "-E";
string COLOR_FLAG = "--color=always";
const string COLOR_START = "[01;31m[K";
const string COLOR_END = "[m[K";

const int error_codes[E_NUM] = {2, 3};

// Helpers
void raise_error(int code);
File open_file(string path);
void parse_args(                 //
    int count, string* args,     //
    bool* color, bool* regex,    //
    File* file, string* pattern  //
);
// IO
string read_line(File file, int* length, bool* end);
void print_hightlighted(int length, string source, List start, List end);
// List functions
List create_list();
void push(List list, int value);
void free_list(List list);
// Strings
void add_char(int* length, string* source, char character);
int get_string_length(string source);
bool equals(string source1, string source2);
// start & end arrays will contain indices of match
bool contains(                         //
    int s_length, string source,       //
    int sub_length, string substring,  //
    List start, List end,              //
    bool should_regex                  //
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
            free(read);
            break;
        }
        List start = create_list();
        List end = create_list();
        bool found = contains(        //
            str_length, read,         //
            pattern_length, pattern,  //
            start, end,               //
            regex                     //
        );
        if (found) {
            exit_code = FOUND;
            if (color) {
                print_hightlighted(str_length, read, start, end);
            } else {
                printf("%s\n", read);
            }
        }
        free_list(start);
        free_list(end);
        free(read);
    }

    fclose(file);
    return exit_code;
}

void print_hightlighted(int length, string source, List start, List end) {
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < start->size; ++j) {
            if (i == start->body[j]) {
                printf("%s", COLOR_START);
            }
        }
        printf("%c", source[i]);
        for (int j = 0; j < end->size; ++j) {
            if (i == end->body[j]) {
                printf("%s", COLOR_END);
            }
        }
    }
    printf("\n");
}

void parse_args(                 //
    int count, string* args,     //
    bool* color, bool* regex,    //
    File* file, string* pattern  //
) {
    *color = false;
    *regex = false;
    for (int i = 0; i < count; ++i) {
        if (equals(args[i], COLOR_FLAG)) {
            *color = true;
        } else if (equals(args[i], REGEX_FLAG)) {
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
    string read = malloc(mem(char, 1));
    forever {
        char character = EOF;
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

bool contains(                         //
    int s_length, string source,       //
    int sub_length, string substring,  //
    List start, List end,              //
    bool should_regex                  //
) {
    for (int i = 0; i < s_length; ++i) {
        int search_pos = i;
        int sub_pos = 0;
        bool found = true;
        int match_end_offset = 0;
        bool repeat = false;
        int repeat_count = 0;
        char repeat_char = '\0';
        forever {
            if (search_pos >= s_length || sub_pos >= sub_length) {
                if (sub_pos + 1 <= sub_length) {
                    found = false;
                }
                if (found) {
                    push(start, i);
                    int final_length = sub_length - match_end_offset;
                    int end_pos = i + final_length + repeat_count - 1;
                    push(end, end_pos);
                }
                break;
            }
            if (repeat) {
                if (source[search_pos] == repeat_char) {
                    repeat_count += 1;
                    search_pos += 1;
                    continue;
                } else {
                    repeat = false;
                    sub_pos += 1;
                    continue;
                }
            }
            if (source[search_pos] != substring[sub_pos]) {
                // Regexp checks
                if (should_regex) {
                    // If current pattern character is `+` or `*`, start
                    // repeating equality checks with previous character
                    if (substring[sub_pos] == '+' ||
                        substring[sub_pos] == '*'  //
                    ) {
                        repeat = true;
                        repeat_char = substring[sub_pos - 1];
                        match_end_offset += 1;
                        continue;
                    }
                    // If current or next pattern symbol is `?` or `*` skip
                    // it
                    if (  //
                        substring[sub_pos] == '?' ||
                        (sub_pos + 1 < sub_length &&
                         (substring[sub_pos + 1] == '?' ||
                          substring[sub_pos + 1] == '*'))  //
                    ) {
                        if (substring[sub_pos] == '?') {
                            match_end_offset += 2;
                        }
                        sub_pos += 1;
                        continue;
                    }
                }
                found = false;
            }
            search_pos += 1;
            sub_pos += 1;
        }
    }
    return start->size != 0;
}

void push(List list, int value) {
    list->size += 1;
    list->body = realloc(list->body, mem(int, list->size));
    list->body[list->size - 1] = value;
}

List create_list() {
    List list = malloc(sizeof(SizedList));
    list->size = 0;
    list->body = malloc(sizeof(int));
    return list;
}

void free_list(List list) {
    free(list->body);
    free(list);
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
