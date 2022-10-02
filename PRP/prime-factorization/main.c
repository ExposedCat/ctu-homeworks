#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void add_to_dict(long* dict, long value, int* size) {
    for (int i = 0; i < *size; i += 2) {
        if (dict[i] == value) {
            dict[i + 1]++;
            return;
        }
    }
    dict[(*size)++] = value;
    dict[(*size)++] = 1;
}

long* get_prime_factors(long n, int* size) {
    int count = 0;
    long* factors = malloc(sizeof(long) * 100);

    while (n % 2 == 0) {
        add_to_dict(factors, 2, &count);
        n = n / 2;
    }
    for (int i = 3; i <= sqrt(n); i = i + 2) {
        while (n % i == 0) {
            add_to_dict(factors, i, &count);
            n = n / i;
        }
    }
    if (n > 2) {
        add_to_dict(factors, n, &count);
    } else if (!count) {
        add_to_dict(factors, 1, &count);
    }

    *size = count;
    return factors;
}

void print_value(int is_first, long value, long power) {
    if (!is_first) {
        printf(" x ");
    }
    if (power > 1) {
        printf("%ld^%ld", value, power);
    } else {
        printf("%ld", value);
    }
}

int throw_invalid_input() {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
}

int main() {
    long value;
    int size = 0;
    while (1) {
        int valid = scanf("%ld", &value);
        if (value == 0) {
            break;
        }
        if (!valid || value < 0) {
            return throw_invalid_input();
        }
        long* factors = get_prime_factors(value, &size);
        printf("Prvociselny rozklad cisla %ld je:\n", value);
        for (int i = 0; i < size; i += 2) {
            print_value(i == 0, factors[i], factors[i + 1]);
        }
        printf("\n");
    }
    return 0;
}
