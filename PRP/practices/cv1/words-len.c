#include <stdio.h>

int main() {
    char symbol;
    int last_is_space = 0;
    int length = 0;
    int is_and_or = 1;
    char last_char = ' ';
    while (1) {
        scanf("%c", &symbol);
        if (symbol == '\n') {
            break;
        }
        if (symbol == '\t' || symbol == ' ' || symbol == '\n') {
            if (!last_is_space) {
                printf(" %d", length);
                if (is_and_or) {
                    printf(" ←");
                }
                printf("\n");
                length = 0;
                last_is_space = 1;
                is_and_or = 1;
            }
        } else {
            last_is_space = 0;
            length += 1;
            printf("> %c\n", last_char);
            if ((last_char == ' ' && symbol != 'a') ||
                (last_char == 'a' && symbol != 'n') ||
                (last_char == 'n' && symbol != 'd')) {
                if ((last_char == ' ' && symbol != 'o') ||
                    (last_char == 'o' && symbol != 'r')) {
                    is_and_or = 0;
                }
            }
            printf("%c", symbol);
        }
        last_char = symbol;
    }
    printf(" %d", length);
    if (is_and_or) {
        printf(" ←");
    }
    printf("\n");
    return 0;
}
