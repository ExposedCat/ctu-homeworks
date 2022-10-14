#include <stdio.h>
#include <stdlib.h>

int main() {
    int sum = 0;
    int count = 0;
    int positive = 0;
    int negative = 0;
    int even = 0;
    int min = 10001;
    int max = -10001;
    int first = 1;
    while (1) {
        int number;
		scanf("%d", &number);

		if (getchar() == EOF) {
			break;
		}

        if (
            number < -10000 || number > 10000 ||
            number < -10000 || number > 10000) {
            printf("\nError: Vstup je mimo interval!\n");
            return 100;
        }

        printf(first ? "%d" : ", %d", number);
        first = 0;

        sum += number;
        count++;
        if (number > 0) {
            positive++;
        } else if (number < 0) {
            negative++;
        }
        if (number % 2 == 0) {
            even++;
        }
        if (number > max) {
            max = number;
        }
        if (number < min) {
            min = number;
        }
    }
    float positive_percent = (float)positive / count * 100;
    float negative_percent = (float)negative / count * 100;

    int odd = count - even;
    float even_percent = (float)even / count * 100;
    float odd_percent = 100 - even_percent;

    float average = (float)sum / count;

    printf("\n");
    printf("Pocet cisel: %d\n", count);
    printf("Pocet kladnych: %d\n", positive);
    printf("Pocet zapornych: %d\n", negative);
    printf("Procento kladnych: %.2f\n", positive_percent);
    printf("Procento zapornych: %.2f\n", negative_percent);
    printf("Pocet sudych: %d\n", even);
    printf("Pocet lichych: %d\n", odd);
    printf("Procento sudych: %.2f\n", even_percent);
    printf("Procento lichych: %.2f\n", odd_percent);
    printf("Prumer: %.2f\n", average);
    printf("Maximum: %d\n", max);
    printf("Minimum: %d\n", min);

    return 0;
}
