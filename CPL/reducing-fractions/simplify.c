/*
    This is a «Reducing Fractions» program.

    There is only one source code file - `main.c`.
    You can compile it with any compiler you like.

    Example compilation via `clang` compiler:
    • Install `clang`
    • Run any terminal emulator
    • Compile program: `clang -pedantic -O2 -Wall -Werror simplify.c -o simplify`
    • Run program: `./simplify` (execute generated binary)
*/

#include <stdio.h>

int calc_gcd(int a, int b) {
    int gcd = 1;
    for (int n = 1; n <= a && n <= b; ++n) {
        if (a % n == 0 && b % n == 0) {
            gcd = n;
        }
    }
    return gcd;
}

int main() {
    int numerator, denumeartor;
    int input_length = scanf("%d / %d", &numerator, &denumeartor);
    if (input_length != 2) {
        fprintf(stderr, "E: Wrong input specified\n");
        return 100;
    }

    int gcd = calc_gcd(numerator, denumeartor);
    int reduced_numerator = numerator / gcd;
    int reduced_denumerator = denumeartor / gcd;
    printf("%d / %d", reduced_numerator, reduced_denumerator);

    return 0;
}
