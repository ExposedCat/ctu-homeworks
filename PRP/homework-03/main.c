#include <stdio.h>
#include <stdlib.h>

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

char* get_wall_char(int transparent, int i, int j) {
    if (transparent) {
        return " ";
    }
    return i % 2 ? (j % 2 ? "o" : "*") : (j % 2 ? "*" : "o");
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
            char* wall = get_wall_char(transparent, i, j);
            char* line = i == 0 || i == height - 1 || j == 0 || j == width - 1 ? "X" : wall;
            printf("%s", line);
        }
        if (fence != -1 && i >= fence_start) {
            if (fence % 2) {
                printf("|");
            }
            char* fence_part = i == fence_start || i == fence + fence_start - 1 ? "-|" : " |";
            for (int i = 0; i < fence / 2; ++i) {
                printf("%s", fence_part);
            }
        }
        printf("\n");
    }
}

int main() {
    int valid;
    int width;
    valid = scanf("%d", &width);
    int height = -1;
    valid = valid && scanf("%d", &height);
    if (!valid) {
        return invalid_input();
    }
    if (width < 3 || width > 69 || height < 3 || height > 69) {
        return input_out_of_range();
    }
    if (width % 2 == 0) {
        return width_is_not_odd();
    }
    int fence = -1;
    if (width == height) {
        valid = scanf("%d", &fence);
        if (!valid || valid == -1) {
            return invalid_input();
        }
        if (fence >= width || !fence) {
            return too_big_fence();
        }
    }
    draw_house(width != height, width, height, fence);
    return 0;
}
