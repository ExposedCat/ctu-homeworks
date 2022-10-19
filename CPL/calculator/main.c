#include <stdio.h>
#include <stdlib.h>

// Nice name
#define bigint char*

// Helpers
#define init_ordered \
    int min, max;    \
    bigint first;    \
    bigint second;
#define order              \
    set_biggest_first(     \
        length1 > length2, \
        length1, number1,  \
        length2, number2,  \
        &min, &max,        \
        &first, &second);

// Argument helpers
#define p_length int* length
#define req_num1 int length1, bigint number1
#define req_num2 int length2, bigint number2

// Error helpers
void raise_invalid_operator() {
    printf("Error: wrong operator\n");
    exit(2);
}

void raise_invalid_operand() {
    printf("Error: wrong operands\n");
    exit(3);
}

// I/O helpers
bigint input_number(p_length) {
    scanf("%d", length);
    // Read newline after length
    getchar();
    bigint number = malloc(sizeof(char) * *length);
    int negative = 0;
    for (int i = 0; i < *length + negative; ++i) {
        char c;
        scanf("%c", &c);
        if (c != '-' && (c < '0' || c > '9')) {
            free(number);
            return NULL;
        }
        if (c == '-') {
            negative = 1;
            number = malloc(sizeof(char) * (*length + 1));
        }
        number[i] = c;
    }
    // Read newline after number
    getchar();
    return number;
}

void print_number(bigint number, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%c", number[i]);
    }
    printf("\n");
}

char input_operator() {
    char operator;
    scanf("%c", &operator);
    if (operator!= '+' && operator!= '-' && operator!= '/' && operator!= '*') {
        return EOF;
    }
    return operator;
}

// Calculation helpers
int is_less(req_num1, req_num2) {
    if (length1 != length2) {
        return length1 < length2;
    }
    for (int i = 0; i < length1; ++i) {
        if (number1[i] == '-') {
            if (number2[i] != '-') {
                return 1;
            }
            continue;
        }
        if (number2[i] == '-') {
            if (number1[i] != '-') {
                return 0;
            }
            continue;
        }
        if (number1[i] > number2[i]) {
            return 0;
        }
    }
    return 1;
}

void set_biggest_first(
    int condition,
    req_num1, req_num2,
    int* min, int* max,
    bigint* first, bigint* second) {
    if (condition) {
        *max = length1;
        *min = length2;
        *first = number1;
        *second = number2;
    } else {
        *max = length2;
        *min = length1;
        *second = number1;
        *first = number2;
    }
}

// Calculator
bigint sum(req_num1, req_num2, p_length) {
    init_ordered;
    order;
    int diff = max - min;
    // Result of the sum may be 1 digit more
    bigint reversed = malloc(sizeof(char) * (max + 1));
    int overload = 0;
    // Sum digits
    for (int i = 0; i < min; ++i) {
        // Char-zero subtraction is necessary for type conversatiob
        int digit1 = first[min - i - 1 + diff] - '0';
        int digit2 = second[min - i - 1] - '0';
        int digit_sum = digit1 + digit2 + overload;
        overload = digit_sum / 10;
        reversed[i] = digit_sum % 10 + '0';
    }
    // Add rest of digits
    for (int i = min; i < max; ++i) {
        int digit = first[max - i - 1] - '0';
        int digit_sum = digit + overload;
        overload = digit_sum / 10;
        reversed[i] = digit_sum % 10 + '0';
    }
    if (overload) {
        reversed[max] = (char)overload + '0';
    }
    *length = max + overload;
    bigint sum = malloc(sizeof(char) * *length);
    // Reverse string
    for (int i = 0; i < *length; ++i) {
        sum[i] = reversed[*length - i - 1];
    }
    free(reversed);
    return sum;
}

bigint subtract(req_num1, req_num2, p_length) {
    // Unique sign cases
    // We do `number += 1` to skip minus sign
    int first_negative = number1[0] == '-';
    int second_negative = number2[0] == '-';
    int negative = 0;
    if (first_negative && second_negative) {
        // -a - -b = b - a
        // Swap
        bigint tmp = number1;
        number1 = number2;
        number2 = tmp;
        number1 += 1;
        number2 += 1;
    } else if (first_negative) {
        // -a - b = -(a + b)
        negative = 1;
        number1 += 1;
    } else if (second_negative) {
        // a - (-b) = a + b
        number2 += 1;
        return sum(length1, number1, length2, number2, length);
    }
    init_ordered;
    // It's important here to compare not only length, but also values
    set_biggest_first(
        is_less(length2, number2, length1, number1),
        length1, number1,
        length2, number2,
        &min, &max,
        &first, &second);
    int diff = max - min;
    // Result of the sum may be negative
    bigint reversed = malloc(sizeof(char) * (max + 1));
    for (int i = 0; i < min; ++i) {
        int position1 = min - i - 1 + diff;
        // Char-zero subtraction is necessary for type conversatiob
        int digit1 = first[position1] - '0';
        int digit2 = second[min - i - 1] - '0';
        if (digit1 < digit2) {
            digit1 += 10;
            // We can take from more than one digit
            for (int i = 1;; ++i) {
                if (first[position1 - i] != '0') {
                    // Found nearest non-zero digit
                    first[position1 - i] = first[position1 - i] - 1;
                    break;
                }
                first[position1 - i] = '9';
            }
        }
        int digit_diff = digit1 - digit2;
        reversed[i] = digit_diff + '0';
    }
    for (int i = min; i < max; ++i) {
        reversed[i] = first[max - i - 1];
    }
    // `+ negative` to optionally add one more `char` for minus sign
    *length = max + negative;
    bigint subtraction = malloc(sizeof(char) * *length);
    // Reverse string
    for (int i = 0; i < *length; ++i) {
        if (!i && negative) {
            subtraction[0] = '-';
            continue;
        }
        subtraction[i] = reversed[*length - i - 1];
    }
    int final_length = *length;
    // Drop zeroes
    for (int i = 0; i < *length; ++i) {
        if (subtraction[i] != '0') {
            break;
        }
        subtraction++;
        final_length -= 1;
    }
    *length = final_length;
    free(reversed);
    return subtraction;
}

// Function to multiply `bigint` only by `int` (1 digit)
// Useful since we multiply digit-by-digit
bigint multiply_int(req_num1, int value, p_length) {
    bigint reversed = malloc(sizeof(char) * (length1 + 1));
    // We need `overload` here to add next digit if result will be > 9
    int overload = 0;
    for (int i = 0; i < length1; ++i) {
        int digit = number1[length1 - i - 1] - '0';
        int result = digit * value + overload;
        overload = result / 10;
        reversed[i] = result % 10 + '0';
    }
    *length = length1 + (overload ? 1 : 0);
    bigint multiple = malloc(sizeof(char) * *length);
    // Reverse string
    for (int i = 0; i < *length; ++i) {
        if (!i && overload) {
            // Add digit if last result was > 9
            multiple[0] = overload + '0';
            continue;
        }
        multiple[i] = reversed[*length - i - 1];
    }
    free(reversed);
    return multiple;
}

bigint multiply(req_num1, req_num2, p_length) {
    init_ordered;
    order;

    // Validate sign
    // `pointer += 1` shifts the pointer so we get rid of sign
    int sign = 1;
    if (first[0] == '-') {
        first += 1;
        sign *= -1;
    }
    if (second[0] == '-') {
        second += 1;
        sign *= -1;
    }

    bigint multiple = NULL;
    for (int i = 0; i < min; ++i) {
        int digit = second[min - i - 1] - '0';
        int next_length;
        bigint next = multiply_int(max, first, digit, &next_length);
        if (!multiple) {
            multiple = next;
            *length = next_length;
        } else {
            // When we multiply each next result should be shifted
            for (int j = 0; j < i; ++j) {
                next = realloc(next, sizeof(char) * (next_length + 1));
                next[next_length++] = '0';
            }
            // Sum up
            bigint next_sum = sum(*length, multiple, next_length, next, length);
            free(multiple);
            multiple = next_sum;
        }
    }
    // Add sign if expression result is negative
    if (sign < 0) {
        multiple = realloc(multiple, sizeof(char) * (*length + 1));
        for (int i = *length; i > 0; --i) {
            multiple[i] = multiple[i - 1];
        }
        multiple[0] = '-';
        *length += 1;
    }
    return multiple;
}

// Uses subtaction so may be too slow :(
bigint divide(req_num1, req_num2, p_length) {
    init_ordered;
    set_biggest_first(
        is_less(length2, number2, length1, number1),
        length1, number1,
        length2, number2,
        &min, &max,
        &first, &second);
    *length = 0;
    bigint result = malloc(sizeof(char));
    print_number(first, max);
    printf("/\n");
    print_number(second, min);
    while (is_less(min, second, max, first)) {
        first = subtract(max, first, min, second, &max);
        result = sum(*length, result, 1, "1", length);
    }
    return result;
}

bigint calculate(req_num1, char operation, req_num2, p_length) {
    switch (operation) {
        case '+':
            return sum(length1, number1, length2, number2, length);
        case '-':
            return subtract(length1, number1, length2, number2, length);
        case '*':
            return multiply(length1, number1, length2, number2, length);
        case '/':
            return divide(length1, number1, length2, number2, length);
        default:
            return number1;
    }
}

int main() {
    // Enter first operand
    int length1;
    bigint number1 = input_number(&length1);
    if (number1 == NULL) {
        raise_invalid_operand();
    }

    // Enter operator
    char operator= input_operator();
    if (operator== EOF) {
        free(number1);
        raise_invalid_operator();
    }

    // Enter second operand
    int length2;
    bigint number2 = input_number(&length2);
    if (number2 == NULL) {
        free(number1);
        raise_invalid_operand();
    }

    // Calculate expression
    int length;
    bigint result = calculate(length1, number1, operator, length2, number2, &length);
    print_number(result, length);

    // Free memory
    if (result != number1) {
        free(result);
    }
    free(number1);
    free(number2);
    return 0;
}
