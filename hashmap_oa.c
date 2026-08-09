// hashmap_oa.c
//
// Implements a Hash Map with open addressing for collisions. Uses a dynamic
// integer array as the underlying data structure
//
// Author: Taylor Cole
#include <stdio.h>
#include <string.h>
#include "int_da.c"


/**
 * Basic hash function for testing
 */
int hash_function_1(char* s, int len)
{
    int hash = 0;
    for (int i = 0; i < len; i++) {
        char letter = *(s + i);
        hash += (int)letter;
    }
    return hash;
}

/**
 * Basic hash function for testing
 */
int hash_function_2(char* s, int len)
{
    int hash = 0;
    for (int i = 0; i < len; i++) {
        char letter = *(s + i);
        hash += (i + 1) * (int)letter;
    }
    return hash;
}

int main()
{
    char* test = "This is a test";
    int result = hash_function_2(test, strlen(test));
    printf("%d", result);
    return 0;
}
