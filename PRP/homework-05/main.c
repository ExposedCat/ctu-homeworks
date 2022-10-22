#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define loop_i(limit) for (int i = 0; i < limit; ++i)
#define loop_j(limit) for (int j = 0; j < limit; ++j)

int required_task(  //
    char* encoded, int encoded_length, char* original,
    int original_length  //
);
int min(int x, int y, int z);
int raise_invalid_input();
int raise_invalid_length();
void clean(char* encoded, char* original);
int calc_difference(char* string1, int length1, char* string2, int length2);
int validate_input(char character);
void shift_string(char* string, int length, int offset);
int calc_same_chars(char* string1, char* string2, int length);
int optional_task(  //
    char* encoded, int encoded_length, char* original,
    int original_length  //
);
int required_task(  //
    char* encoded, int encoded_length, char* original,
    int original_length  //
);

int main(int argc, char* argv[]) {
    short optional = argc == 2 && !strcmp(argv[1], "-prp-optional");

    char* encoded = malloc(sizeof(char) * 2);
    int encoded_length = 0;
    char next;
    while (1) {
        scanf("%c", &next);
        int action = validate_input(next);
        if (action == 1) {
            break;
        } else if (action) {
            clean(encoded, NULL);
            return action;
        }
        encoded[encoded_length++] = next;
        encoded = realloc(encoded, sizeof(char) * (encoded_length + 1));
    }

    char* original = malloc(sizeof(char));
    int original_length = 0;
    while (1) {
        scanf("%c", &next);
        int action = validate_input(next);
        if (action == 1) {
            break;
        } else if (action) {
            clean(encoded, original);
            return action;
        }
        original[original_length++] = next;
        original = realloc(original, sizeof(char) * (original_length + 1));
    }

    int exit_code = 0;

    if (optional) {
        exit_code = optional_task(     //
            encoded, encoded_length,   //
            original, original_length  //
        );
    } else {
        exit_code = required_task(     //
            encoded, encoded_length,   //
            original, original_length  //
        );
    }

    clean(encoded, original);
    return exit_code;
}

int min(int x, int y, int z) {
    return (x < y ? (x < z ? x : z) : (y < z ? y : z));
}

int raise_invalid_input() {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
}

int raise_invalid_length() {
    fprintf(stderr, "Error: Chybna delka vstupu!\n");
    return 101;
}

void clean(char* encoded, char* original) {
    free(encoded);
    if (original) {
        free(original);
    }
}

// Calculate Levenstein distance
int calc_difference(char* string1, int length1, char* string2, int length2) {
    int length = length2 + 1;
    int above[length], below[length];
    for (int *k = above, c = 0; k < above + length; ++k, ++c) {
        *k = c;
    }

    char* string1_pos = string1;
    char* string2_pos;
    for (int j = 1; j < length1 + 1; ++j) {
        *below = j;
        string2_pos = string2;
        int* up = above;
        int* up1 = above + 1;
        int* down = below;
        int* down1 = below + 1;
        for (; down1 < below + length; ++up, ++up1, ++down, ++down1) {
            int a = *string1_pos == *string2_pos ? *up : *up + 1;
            int b = *up1 + 1;
            int c = *down + 1;
            *down1 = min(a, b, c);
            string2_pos++;
        }
        up = above;
        down = below;
        for (; up < above + length; ++up, ++down) {
            *up = *down;
        }
        string1_pos++;
    }

    return above[length - 1];
}

int validate_input(char character) {
    if (character == '\n') {
        return 1;
    }
    if (                                         //
        character < 'A' ||                       //
        (character > 'Z' && character < 'a') ||  //
        character > 'z'                          //
    ) {
        return raise_invalid_input();
    }
    return 0;
}

void shift_string(char* string, int length, int offset) {
    loop_i(length) {
        loop_j(offset) {
            if (string[i] == 'Z') {
                string[i] = 'a';
            } else if (string[i] == 'z') {
                string[i] = 'A';
            } else {
                string[i]++;
            }
        }
    }
}

int calc_same_chars(char* string1, char* string2, int length) {
    int same = 0;
    loop_i(length) {  //
        same += string1[i] == string2[i];
    }
    return same;
}

int optional_task(  //
    char* encoded, int encoded_length, char* original,
    int original_length  //
) {
    int position = 0;
    int min_difference = encoded_length + 1;
    loop_i(52) {
        shift_string(encoded, encoded_length, 1);
        int difference = calc_difference(  //
            encoded, encoded_length,       //
            original, original_length      //
        );
        if (difference < min_difference) {
            min_difference = difference;
            position = i;
        }
    }

    shift_string(encoded, encoded_length, position + 1);
    loop_i(encoded_length) {  //
        printf("%c", encoded[i]);
    }
    printf("\n");
    return 0;
}

int required_task(  //
    char* encoded, int encoded_length, char* original,
    int original_length  //
) {
    if (encoded_length != original_length) {
        return raise_invalid_length();
    }

    int position = 0;
    int max_score = -1;
    loop_i(52) {
        shift_string(encoded, encoded_length, 1);
        int same = calc_same_chars(original, encoded, encoded_length);
        if (same > max_score) {
            max_score = same;
            position = i;
        }
    }

    shift_string(encoded, encoded_length, position + 1);
    loop_i(encoded_length) {  //
        printf("%c", encoded[i]);
    }
    printf("\n");
    return 0;
}
