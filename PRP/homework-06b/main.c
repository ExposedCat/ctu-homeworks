#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors {
    INVALID_INPUT
};

typedef struct Matrix {
    int n;
    int m;
    int negative;
    char name;
    int** body;
} Matrix;

char* error_hints[1] = {
    "Error: Chybny vstup!\n"};

int error_codes[1] = {
    100};

void raise_error(int code) {
    fprintf(stderr, "%s", error_hints[code]);
    exit(error_codes[code]);
}

Matrix** push(Matrix** list, int* size, Matrix* matrix) {
    list = realloc(list, sizeof(Matrix*) * (*size + 1));
    list[*size] = matrix;
    *size += 1;
    return list;
}

void free_matrix(Matrix* matrix, int size) {
    for (int i = 0; i < size; ++i) {
        free(matrix->body[i]);
    }
    free(matrix->body);
    free(matrix);
}

Matrix* input_matrix(int* created) {
    Matrix* matrix = malloc(sizeof(Matrix));
    int count = scanf("%c", &matrix->name);
    if (!count || matrix->name == '\n') {
        free(matrix);
        *created = 0;
        return NULL;
    }
    matrix->negative = 0;
    matrix->n = 0;
    matrix->m = 0;
    int i = 0;
    matrix->body = malloc(sizeof(int*));
    while (1) {
        size_t new_size = sizeof(int*) * (i + 2);
        matrix->body = realloc(matrix->body, new_size);

        matrix->body[i] = malloc(sizeof(int));
        int j = 0;
        int end = 0;
        while (1) {
            size_t new_size = sizeof(int) * (j + 2);
            matrix->body[i] = realloc(matrix->body[i], new_size);
            char next;
            scanf("%c", &next);
            if (next == '\n') {
                free(matrix);
                *created = 0;
                return NULL;
            }
            if (next == ';' || next == ']') {
                if (!matrix->m) {
                    matrix->m = j;
                }
                if (next == ']') {
                    end = 1;
                    getchar();
                }
                break;
            }
            if (next == '=') {
                getchar();
            }
            scanf("%d", &matrix->body[i][j]);
            j += 1;
        }
        matrix->body[i] = realloc(matrix->body[i], sizeof(int) * matrix->m);
        i += 1;
        if (end) {
            matrix->n = i;
            break;
        }
    }
    matrix->body = realloc(matrix->body, sizeof(int*) * matrix->n);
    *created = 1;
    return matrix;
}

void print_matrix(Matrix* matrix) {
    printf("[");
    for (int i = 0; i < matrix->n; ++i) {
        for (int j = 0; j < matrix->m; ++j) {
            printf("%d", matrix->body[i][j]);
            if (j != matrix->m - 1) {
                printf(" ");
            }
        }
        if (i != matrix->n - 1) {
            printf("; ");
        }
    }
    printf("]\n");
}

void free_list(Matrix** list, int size, int free_self) {
    for (int i = 0; i < size; ++i) {
        free_matrix(list[i], list[i]->n);
    }
    if (free_self) {
        free(list);
    }
}

Matrix* mirror(Matrix* matrix) {
    Matrix* negative = malloc(sizeof(Matrix));
    negative->n = matrix->n;
    negative->m = matrix->m;
    negative->body = malloc(sizeof(int*) * matrix->n);
    for (int i = 0; i < matrix->n; ++i) {
        negative->body[i] = malloc(sizeof(int) * matrix->m);
        for (int j = 0; j < matrix->m; ++j) {
            negative->body[i][j] = -matrix->body[i][j];
        }
    }
    return negative;
}

Matrix* sum_matrices(Matrix* matrix1, Matrix* matrix2) {
    if (
        matrix1->n != matrix2->n ||
        matrix1->m != matrix2->m) {
        return NULL;
    }
    Matrix* sum = malloc(sizeof(Matrix));
    sum->negative = 0;
    sum->n = matrix1->n;
    sum->m = matrix1->m;
    sum->body = malloc(sizeof(int*) * sum->n);
    for (int i = 0; i < sum->n; ++i) {
        sum->body[i] = malloc(sizeof(int) * sum->m);
        for (int j = 0; j < sum->m; ++j) {
            sum->body[i][j] = matrix1->body[i][j] + matrix2->body[i][j];
        }
    }
    return sum;
}

Matrix* multiply_matrices(Matrix* matrix1, Matrix* matrix2) {
    if (matrix1->m != matrix2->n) {
        return NULL;
    }
    Matrix* product = malloc(sizeof(Matrix));
    product->negative = 0;
    product->n = matrix1->n;
    product->m = matrix2->m;
    product->body = malloc(sizeof(int*) * product->n);
    for (int i = 0; i < product->n; ++i) {
        product->body[i] = malloc(sizeof(int) * product->m);
        for (int j = 0; j < product->m; ++j) {
            product->body[i][j] = 0;
            for (int k = 0; k < matrix1->m; ++k) {
                product->body[i][j] += matrix1->body[i][k] * matrix2->body[k][j];
            }
        }
    }
    return product;
}

Matrix* calculate(Matrix** list, int size, char operation) {
    Matrix* calculated = list[0];
    for (int i = 1; i < size; ++i) {
        Matrix* old_calculated = calculated;
        if (operation == '*') {
            calculated = multiply_matrices(calculated, list[i]);
        } else {
            calculated = sum_matrices(calculated, list[i]);
        }
        if (old_calculated != list[0]) {
            free_matrix(old_calculated, old_calculated->n);
        }
        if (calculated == NULL) {
            free_list(list, size, 1);
            raise_error(INVALID_INPUT);
        }
    }
    return calculated;
}

Matrix* get_matrix(Matrix** list, int size, char name) {
    for (int i = 0; i < size; ++i) {
        if (list[i]->name == name) {
            return list[i];
        }
    }
    return NULL;
}

int main() {
    int size = 0;
    Matrix** matrices = malloc(sizeof(Matrix**));
    int f_size = 0;
    Matrix** for_free = malloc(sizeof(Matrix**));

    while (1) {
        int created;
        Matrix* matrix = input_matrix(&created);
        if (!created) {
            break;
        }
        matrices = push(matrices, &size, matrix);
    }

    int m_size = 0;
    Matrix** for_multiplication = malloc(sizeof(Matrix**));
    int s_size = 0;
    Matrix** for_sum = malloc(sizeof(Matrix**));

    int should_mirror = 0;
    int switch_sum = 0;
    while (1) {
        char name;
        char operation;
        scanf("%c%c", &name, &operation);
        Matrix* matrix = get_matrix(matrices, size, name);
        if (operation != '*') {
            if (should_mirror) {
                matrix = mirror(matrix);
                matrices = push(matrices, &size, matrix);
                should_mirror = 0;
            }
            if (operation == '-') {
                should_mirror = 1;
            }
            if (switch_sum) {
                for_multiplication = push(for_multiplication, &m_size, matrix);
                switch_sum = 0;
            } else {
                for_sum = push(for_sum, &s_size, matrix);
            }
            if (m_size > 1) {
                Matrix* calculated = calculate(for_multiplication, m_size, '*');
                m_size = 0;
                for_sum = push(for_sum, &s_size, calculated);
                for_free = push(for_free, &f_size, calculated);
            }
        } else {
            for_multiplication = push(for_multiplication, &m_size, matrix);
            switch_sum = 1;
        }
        if (operation == '\n') {
            break;
        }
    }

    free(for_multiplication);

    // Calculate sum
    Matrix* sum = NULL;
    if (s_size) {
        sum = for_sum[0];
        if (s_size > 1) {
            sum = calculate(for_sum, s_size, '+');
        }
    }

    if (sum) {
        print_matrix(sum);
        free_matrix(sum, sum->n);
    }

    free_list(matrices, size, 1);
    free_list(for_free, f_size, 1);
    free(for_sum);

    return 0;
}
