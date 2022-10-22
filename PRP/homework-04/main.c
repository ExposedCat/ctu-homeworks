#include <stdio.h>
#include <stdlib.h>

// Number size restrictions
#define MAX_PRIME 1e6

// Helpers
int throw_invalid_input(int* sieve) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    free(sieve);
    return 100;
}

// Services
void print_factor(int value, int power, int add_x) {
    if (value == 1) {
        printf("1\n");
        return;
    }
    if (power > 1) {
        printf("%d^%d", value, power);
    } else {
        printf("%d", value);
    }
    if (add_x) {
        printf(" x ");
    }
}

void print_prime_factors(int* sieve, int sieve_size, long n) {
    printf("Prvociselny rozklad cisla %ld je:\n", n);
    if (n == 1) {
        print_factor(1, 1, 0);
        return;
    }
    int factor = 0;
    int power = 0;
    for (int i = 0; factor < n && i < sieve_size; ++i) {
        int prime = sieve[i];
        if (!prime) {
            break;
        }
        while (n % prime == 0) {
            if (prime == factor) {
                power++;
            } else {
                // Found next factor, print previous
                // if it's not the first one
                if (factor) {
                    print_factor(factor, power, 1);
                }
                // Reset counters
                factor = prime;
                power = 1;
            }
            n /= prime;
        }
    }
    // Print last factor
    print_factor(factor ? factor : n, power ? power : 1, 0);
    printf("\n");
}

int* get_prime_numbers_sieve(int* sieve_size) {
    // Create binary list of prime numbers
    int* numbers = malloc(sizeof(int) * MAX_PRIME);
    for (int i = 2; i < MAX_PRIME; ++i) {
        numbers[i] = 1;
    }
    for (int i = 2; i < MAX_PRIME; ++i) {
        if (numbers[i]) {
            (*sieve_size)++;
            for (long j = (long)i * i; j < MAX_PRIME; j += i) {
                numbers[j] = 0;
            }
        }
    }

    // Create pure list of prime numbers
    int j = 0;
    int* sieve = malloc(sizeof(int) * (*sieve_size));
    for (int i = 2; i < MAX_PRIME; ++i) {
        if (numbers[i]) {
            sieve[j++] = i;
        }
    }
    // Clean extra memory
    free(numbers);
    sieve = realloc(sieve, sizeof(int) * (*sieve_size));

    return sieve;
}

int main() {
    int sieve_size = 0;
    int* sieve = get_prime_numbers_sieve(&sieve_size);

    long value = 0;
    while (1) {
        int valid = scanf("%ld", &value);
        if (value == 0) {
            break;
        }
        if (!valid || value < 0) {
            return throw_invalid_input(sieve);
        }
        print_prime_factors(sieve, sieve_size, value);
    }

    free(sieve);
    return 0;
}
