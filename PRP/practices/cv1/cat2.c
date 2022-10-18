#include <ctype.h>
#include <stdio.h>

int main() {
    char symbol;
    int space_found = 0;
    while (1) {
        scanf("%c", &symbol);
        if (symbol == '\n') {
            break;
        }
        if (isspace(symbol)) {
            space_found = 1;
            continue;
        }
        if (space_found) {
            printf("%c", symbol);
        }
    }
    printf("\n");
    return 0;
}
