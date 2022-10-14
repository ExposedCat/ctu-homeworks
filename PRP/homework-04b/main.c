#include <stdio.h>
#include <stdlib.h>

#define HUNDRED 101
#define MAX 1e6

int *get_prime_factors_sieve(int *sieve_size) {
    int *numbers = malloc(sizeof(int) * MAX);
    for (int i = 2; i < MAX; ++i) {
        numbers[i] = 1;
    }

    for (int i = 2; i < MAX; ++i) {
        if (numbers[i]) {
            (*sieve_size)++;
            for (long j = (long)i * i; j < MAX; j += i) {
                numbers[j] = 0;
            }
        }
    }

    int j = 0;
    int *sieve = malloc(sizeof(int) * (*sieve_size));
    for (int i = 2; i < MAX; ++i) {
        if (numbers[i]) {
            sieve[j++] = i;
        }
    }
    free(numbers);
    return realloc(sieve, sizeof(int) * (*sieve_size));
}

int get_num_length(int number) {
    int length = 0;
    while (number) {
        length++;
        number /= 10;
    }
    return length;
}

void take(int *part, short **array, int input_index, int index) {
    *part *= 10;
    *part += array[input_index][index];
}

short ensure_divides(short **array, int *input, int input_index, int prime) {
    int prime_length = get_num_length(prime);
    int input_length = input[input_index];

    short result_length = -1;
    short *result = calloc(input[input_index], sizeof(short));

    int index = prime_length - 1;
    int part = 0;
    for (int i = 0; i < prime_length; ++i) {
        take(&part, array, input_index, i);
    }
    if (part < prime) {
        take(&part, array, input_index, prime_length);
        index++;
    }

    while (index != input_length) {
        int count = 0;
        while (part >= prime) {
            part -= prime;
            count++;
        }
        result[++result_length] = count;
        index++;
        if (index != input_length) {
            take(&part, array, input_index, index);
        }
    }

    int exit = 0;
    if (!part) {
        for (int i = 0; i < input[input_index]; ++i) {
            array[input_index][i] = (i <= result_length) ? result[i] : 0;
        }
        input[input_index] = result_length + 1;
        exit = 1;
    }

    free(result);
    return exit;
}

short ensure_one(short **array, int *input, int input_index) {
    for (int i = 0; i < input[input_index] - 1; ++i) {
        if (array[input_index][i] != 0) {
            return 0;
        }
    }
    if (array[input_index][input[input_index] - 1] != 1) {
        return 0;
    }
    return 1;
}

void print_factor(int value, int power, int add_x) {
    if (!value) {
        return;
    }
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

void print_factors(int *sieve, int sieve_size, short **array, int *input, int input_index) {
    printf("Prvociselny rozklad cisla ");
    for (int i = 0; i < input[input_index]; ++i) {
        printf("%d", array[input_index][i]);
    }
    printf(" je:\n");
    if (input[input_index] == 1 && array[input_index][0] == 1) {
        print_factor(1, 1, 0);
        return;
    }
    int number = 0;
    int power = 0;
    while (!ensure_one(array, input, input_index)) {
        for (int i = 0; i < sieve_size; ++i) {
            if (ensure_divides(array, input, input_index, sieve[i]) == 1) {
                if (sieve[i] == number) {
                    power++;
                } else {
                    print_factor(number, power, 1);
                    number = sieve[i];
                    power = 1;
                }
                break;
            }
        }
    }
    print_factor(number, power, 0);
    printf("\n");
}

int main() {
    int exit_code = 0;

    int sieve_size = 0;
    int *sieve = get_prime_factors_sieve(&sieve_size);

    int *input = calloc(HUNDRED, sizeof(int));
    short **array = malloc(HUNDRED * sizeof(short *));
    for (int i = 0; i < HUNDRED; ++i) {
        array[i] = calloc(HUNDRED, sizeof(short));
    }

    int index = 0;
    while (1) {
        char next = fgetc(stdin);
        if (next == '\n') {
            print_factors(sieve, sieve_size, array, input, index++);
            continue;
        } else {
            if (next == '0' && input[index] == 0) {
                break;
            }
            if (next > '9' || next < '0') {
                fputs("Error: Chybny vstup!\n", stderr);
                exit_code = 100;
    			break;
            }
            array[index][input[index]++] = (int)(next - '0');
        }
    }

    free(sieve);
    free(input);
    for (int i = 0; i < HUNDRED; ++i) {
        free(array[i]);
    }
    free(array);
    return exit_code;
}
