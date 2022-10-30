#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"

void test_strspn(                           //
    const char* first, const char* second,  //
    unsigned long expected                  //
) {
    printf("• Input: «%s», «%s»\n", first, second);
    printf("· Expected: %lu\n", expected);
    unsigned long count = strspn(first, second);
    printf("· Actual: %lu\n", count);
}

void test_strcat(                 //
    char** dest, const char* src  //
) {
    printf("• Input: «%s», «%s»\n", *dest, src);
    printf("· Expected: «%s%s»\n", *dest, src);
    strcat(dest, src);
    printf("· Actual: «%s»\n", *dest);
}

void test_strtok(                 //
    char* str, const char* delim  //
) {
    printf("• Input: «%s» / «%s»\n", str, delim);
    printf("· Outputs:\n");
    char* token = strtok(str, delim);
    while (token != NULL) {
        printf(" - «%s»\n", token);
        token = strtok(NULL, delim);
    }
    printf("· End\n");
}

void test_strstr_match(                  //
    const char* str, const char* substr  //
) {
    printf("• Input: «%s», «%s»\n", str, substr);
    const char* match = NULL;
    unsigned long length = strstr_match(str, substr, &match);
    printf(" - «%s»[%lu]\n", match, length);
}

int main() {
    // strspn
    printf("Test strspn\n");

    test_strspn("123abc", "0987654321", 3);

    test_strspn("a123bc", "0987654321", 0);

    // stract
    printf("Test stract\n");

    char* strcat_dest1 = malloc(sizeof(char) * 17);
    const char* strcat_dest1_src = "123abc";
    for (int i = 0; i < 17; ++i) {
        strcat_dest1[i] = strcat_dest1_src[i];
    }
    test_strcat(&strcat_dest1, "0987654321");
    free(strcat_dest1);

    char* strcat_dest2 = malloc(sizeof(char) * 17);
    const char* strcat_dest2_src = "a123bc";
    for (int i = 0; i < 17; ++i) {
        strcat_dest2[i] = strcat_dest2_src[i];
    }
    test_strcat(&strcat_dest2, "0987654321");
    free(strcat_dest2);

    // strtok
    printf("Test strtok\n");

    char* strtok_dest1 = malloc(sizeof(char) * 10);
    const char* strtok_dest1_src = "abc1_abc2";
    for (int i = 0; i < 10; ++i) {
        strtok_dest1[i] = strtok_dest1_src[i];
    }
    test_strtok(strtok_dest1, "_");
    free(strtok_dest1);

    char* strtok_dest2 = malloc(sizeof(char) * 15);
    const char* strtok_dest2_src = "abc1_abc2#abc3";
    for (int i = 0; i < 15; ++i) {
        strtok_dest2[i] = strtok_dest2_src[i];
    }
    test_strtok(strtok_dest2, "_#");
    free(strtok_dest2);

    char* strtok_dest3 = malloc(sizeof(char) * 15);
    const char* strtok_dest3_src = "abc1xabc2xabc3";
    for (int i = 0; i < 15; ++i) {
        strtok_dest3[i] = strtok_dest3_src[i];
    }
    test_strtok(strtok_dest3, "_#");
    free(strtok_dest3);

    // strstr_match
    printf("Test strstr_match\n");

    test_strstr_match("abca", "a");
    test_strstr_match("abca", "b");
    test_strstr_match("abcax", "c[ab]");
    test_strstr_match("1234567", "4[35]6[79]");
    test_strstr_match("abca", "d");

    return 0;
}
