#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors {
    INVALID_INPUT
};

typedef struct Matrix {
    int n;
    int m;
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

Matrix* input_matrix(int* created, char* operation) {
    Matrix* matrix = malloc(sizeof(Matrix));
    int count = scanf("%d %d", &matrix->n, &matrix->m);
    if (count != 2) {
        free(matrix);
        *created = 0;
        return NULL;
    }
    matrix->body = malloc(sizeof(int*) * matrix->n);
    for (int i = 0; i < matrix->n; ++i) {
        matrix->body[i] = malloc(sizeof(int) * matrix->m);
        for (int j = 0; j < matrix->m; ++j) {
            int count = scanf("%d", &matrix->body[i][j]);
            if (!count) {
                free_matrix(matrix, i + 1);
                *created = 0;
                return NULL;
            }
        }
    }
    count = scanf("%*c%c", operation);
    if (count == -1) {
        *operation = EOF;
    }
    if (
        *operation != EOF &&
        *operation != '+' &&
        *operation != '-' &&
        *operation != '*') {
        free_matrix(matrix, matrix->n);
        *created = 0;
        return NULL;
    }
    *created = 1;
    return matrix;
}

void print_matrix(Matrix* matrix) {
    printf("%d %d\n", matrix->n, matrix->m);
    for (int i = 0; i < matrix->n; ++i) {
        for (int j = 0; j < matrix->m; ++j) {
            printf("%d", matrix->body[i][j]);
            if (j != matrix->m - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void free_list(Matrix** list, int size, int free_self) {
    for (int i = 0; i < size; ++i) {
        free_matrix(list[i], list[i]->n);
    }
    if (free_self) {
        free(list);
    }
}

void mirror(Matrix* matrix) {
    for (int i = 0; i < matrix->n; ++i) {
        for (int j = 0; j < matrix->m; ++j) {
            matrix->body[i][j] = -matrix->body[i][j];
        }
    }
}

Matrix* sum_matrices(Matrix* matrix1, Matrix* matrix2) {
    if (
        matrix1->n != matrix2->n ||
        matrix1->m != matrix2->m) {
        return NULL;
    }
    Matrix* sum = malloc(sizeof(Matrix));
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
        if (i != 1) {
            free_matrix(old_calculated, old_calculated->n);
        }
        if (calculated == NULL) {
            free_list(list, size, 1);
            raise_error(INVALID_INPUT);
        }
    }
    return calculated;
}

int main() {
    int m_size = 0;
    Matrix** for_multiplication = malloc(sizeof(Matrix**));
    int s_size = 0;
    Matrix** for_sum = malloc(sizeof(Matrix**));

    int should_mirror = 0;
    int switch_sum = 0;
    char operation;
    do {
        int created;
        Matrix* matrix = input_matrix(&created, &operation);

        if (!created) {
            free_list(for_multiplication, m_size, 1);
            free_list(for_sum, s_size, 1);
            raise_error(INVALID_INPUT);
        }
        if (operation != '*') {
            if (should_mirror) {
                mirror(matrix);
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
                free_list(for_multiplication, m_size, 0);
                m_size = 0;
                for_sum = push(for_sum, &s_size, calculated);
            }
        } else {
            for_multiplication = push(for_multiplication, &m_size, matrix);
            switch_sum = 1;
        }
    } while (operation != EOF);

    free_list(for_multiplication, m_size, 1);

    // Calculate sum
    Matrix* sum = NULL;
    if (s_size) {
        sum = for_sum[0];
        if (s_size > 1) {
            sum = calculate(for_sum, s_size, '+');
        }
    }

    print_matrix(sum);

    free_list(for_sum, s_size, 1);
    if (s_size > 1) {
        free_matrix(sum, sum->n);
    }

    return 0;
}
