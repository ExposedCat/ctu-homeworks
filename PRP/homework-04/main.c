#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void print_factor(long value, long power, int add_x) {
    if (value == 1) {
        printf("1\n");
        return;
    }
    if (power > 1) {
        printf("%ld^%ld", value, power);
    } else {
        printf("%ld", value);
    }
    if (add_x) {
        printf(" x ");
    }
}

void print_prime_factors(long* sieve, long sieve_size, long n) {
    printf("Prvociselny rozklad cisla %ld je:\n", n);
    if (n == 1) {
        print_factor(1, 1, 0);
        return;
    }
    long factor = 0;
    int power = 0;
    for (int i = 0; factor < n; ++i) {
        long prime = sieve[i];
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

long* get_prime_factors_sieve(long limit, long* size) {
    long* numbers = malloc(sizeof(long) * limit);
    for (long i = 2; i < limit; ++i) {
        numbers[i] = 1;
    }

    for (long i = 2; i < limit; ++i) {
        if (numbers[i]) {
            for (long j = i * i; j < limit; j += i) {
                numbers[j] = 0;
            }
        }
    }

    long factors_size = 0;
    long* factors = malloc(sizeof(long) * limit);
    for (long i = 2; i < limit; ++i) {
        if (numbers[i]) {
            factors[factors_size++] = i;
        }
    }
    free(numbers);
    factors = realloc(factors, sizeof(long) * factors_size);

    *size = factors_size;
    return factors;
}

int throw_invalid_input(long* sieve) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    free(sieve);
    return 100;
}

int main() {
    long sieve_size;
    long* sieve = get_prime_factors_sieve(1e6, &sieve_size);

    long value;
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
