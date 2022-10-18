#include <ctype.h>
#include <stdio.h>

int main() {
    char symbol;
    int last_is_space = 0;
    while (1) {
        scanf("%c", &symbol);
        if (symbol == '\n') {
            break;
        }
        if (symbol == '\t' || symbol == ' ' || symbol == '\n') {
            if (!last_is_space) {
                printf("\n");
                last_is_space = 1;
            }
        } else {
            last_is_space = 0;
            printf("%c", symbol);
        }
    }
    printf("\n");
    return 0;
}
