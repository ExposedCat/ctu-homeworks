#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define string char*
#define mem(number, type) sizeof(type) * (number)

void input_words(int* count, string** words);
void push(string** list, int* size, string str);
int string_comparator(const void* first, const void* second);

int main() {
    // Input
    int count = 0;
    string* words;
    input_words(&count, &words);

    // Process
    qsort(words, count, sizeof(string), string_comparator);

    // Output
    for (int i = 0; i < count; ++i) {
        printf("%s\n", words[i]);
    }

    // Clear memory
    for (int i = 0; i < count; ++i) {
        // Clear each word
        free(words[i]);
    }
    free(words);
    return 0;
}

void push(string** list, int* size, string str) {
    *size += 1;
    *list = realloc(*list, mem(*size, string*));
    (*list)[*size - 1] = str;
}

void input_words(int* count, string** words) {
    // We do no checks since the task promises a valid input
    int length;
    scanf("%d", &length);
    // Read newline symbol after the number
    getchar();
    *words = malloc(mem(1, string));
    while (1) {
        string word = malloc(mem(length + 1, char));
        int entered = scanf("%s", word);
        if (entered == -1) {
            free(word);
            break;
        }
        push(words, count, word);
    }
}

int string_comparator(const void* first, const void* second) {
    return strcmp(*(string*)first, *(string*)second);
}
