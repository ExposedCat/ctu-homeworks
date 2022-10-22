#include <stdio.h>
#include <stdlib.h>

// House size restrictions
#define MIN_FENCE_SIZE 1
#define MIN_SIZE 3
#define MAX_SIZE 69

// Helpers
int invalid_input();
int input_out_of_range();
int width_is_not_odd();
int too_big_fence();
char get_interior_char(int transparent, int i, int j);

// Services
void draw_house(int transparent, int width, int height, int fence);

int main() {
    int width, height;
    int count = scanf("%d %d", &width, &height);
    if (count != 2) {
        return invalid_input();
    }
    if ((width < MIN_SIZE || width > MAX_SIZE) ||
        (height < MIN_SIZE || height > MAX_SIZE)) {
        return input_out_of_range();
    }
    if (width % 2 == 0) {
        return width_is_not_odd();
    }
    int fence = -1;
    if (width == height) {
        count = scanf("%d", &fence);
        if (count != 1) {
            return invalid_input();
        }
        if (fence >= width || fence < MIN_FENCE_SIZE) {
            return too_big_fence();
        }
    }
    draw_house(width != height, width, height, fence);
    return 0;
}

int invalid_input() {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
}

int input_out_of_range() {
    fprintf(stderr, "Error: Vstup mimo interval!\n");
    return 101;
}

int width_is_not_odd() {
    fprintf(stderr, "Error: Sirka neni liche cislo!\n");
    return 102;
}

int too_big_fence() {
    fprintf(stderr, "Error: Neplatna velikost plotu!\n");
    return 103;
}

char get_interior_char(int transparent, int i, int j) {
    if (transparent) {
        return ' ';
    }
    return i % 2 ? (j % 2 ? 'o' : '*') : (j % 2 ? '*' : 'o');
}

void draw_house(int transparent, int width, int height, int fence) {
    int roof_center = width / 2;
    int roof_height = roof_center;
    for (int i = 0; i < roof_height; ++i) {
        for (int j = 0; j <= roof_center + i; ++j) {
            printf(j == roof_center - i || j == roof_center + i ? "X" : " ");
        }
        printf("\n");
    }
    int fence_start = height - fence;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char interior = get_interior_char(transparent, i, j);
            int is_interior =
                (i == 0 || i == height - 1) || (j == 0 || j == width - 1);
            char next_symbol = is_interior ? 'X' : interior;
            printf("%c", next_symbol);
        }
        if (fence != -1 && i >= fence_start) {
            if (fence % 2) {
                printf("|");
            }
            int is_fence = i == fence_start || i == fence + fence_start - 1;
            char* fence_part = is_fence ? "-|" : " |";
            for (int i = 0; i < fence / 2; ++i) {
                printf("%s", fence_part);
            }
        }
        printf("\n");
    }
}
