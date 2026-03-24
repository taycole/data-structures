/* Static array fundamentals.
   Implements 10 functions that manipulate an array.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Gets the number of elements in an array
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

// Structs
struct min_max {
    int min, max;
};

// Function declarations
void print_int_array(int *arr, int len);
void print_str_array(char **arr, int len);
struct min_max get_min_max(int *arr, int len);
char **fizz_buzz(int *arr, int len);
void free_fizzbuzz(char **ptr_arr, int len);
void reverse(int *arr, int len);


// Used to test out the functions
int main()
{
    struct min_max result;

    // Min max tests ---------------------------------------------------------
    int test1[] = {7, 8, 6, -5, 4};
    int test2[] = {100};
    int test3[] = {3, 3, 3};
    int test4[] = {-10, -30, -5, 0, -10};
    int test5[] = {25, 50, 0, 10};
    
    int *test_cases[] = {test1, test2, test3, test4, test5};
    int test_lens[] = {5, 1, 3, 5, 4};

    // Gets min max of each test, displays original array and result
    for (int i = 0; i < 5; i++) {
        printf("min_max example %d:\n", i + 1);
        print_int_array(test_cases[i], test_lens[i]);
        result = get_min_max(test_cases[i], test_lens[i]);
        printf("Min: %d, Max: %d\n", result.min, result.max);
    }
    // -----------------------------------------------------------------------
    // fizz_buzz test
    printf("\nfizz_buzz test:\n");
    int fb_test[7] = {-5, -1, 3, 7, 11, 15, 19};
    char **fizzbuzzed = fizz_buzz(fb_test, 7);
    
    print_str_array(fizzbuzzed, 7);
    print_int_array(fb_test, 7);

    free_fizzbuzz(fizzbuzzed, 7);
    // -----------------------------------------------------------------------
    // array reverse example 1
    printf("\nreverse test:\n");
    int rev_test[] = {-20, -13, -6, 1, 8, 15};
    print_int_array(rev_test, 6);
    reverse(rev_test, 6);
    print_int_array(rev_test, 6);
    reverse(rev_test, 6);
    print_int_array(rev_test, 6);
    // -----------------------------------------------------------------------
    // rotate tests
    printf("\nrotate example 1");
    
    
    
    return 0;
}

// Prints each element of an integer array
void print_int_array(int *arr, int len)
{
    printf("Array: { ");
    for (int i = 0; i < len; i++) {
	printf("%d ", *(arr + i));
    }
    printf("}\n");
}

// Prints each element of an array of strings
void print_str_array(char **arr, int len)
{
    printf("Str Array { ");
    for (int i = 0; i < len; i++) {
	printf("%s ", arr[i]);
    }
    printf("}\n");
}


/**
 * Finds and returns the minimum and maximum of a static array.
 *
 * @param arr: Static array of integer values.
 * @param len: Length of the array 
 *
 * returns: Struct containing the mininum and maximum values.
 */
struct min_max get_min_max(int *arr, int len)
{
    int current_min = arr[0], current_max = arr[0], curr_val;
    for (int i = 1; i < len; i++) {
        curr_val = arr[i];
        if (curr_val > current_max)
            current_max = curr_val;
        if (curr_val < current_min)
	    current_min = curr_val;
    }
    struct min_max result = {current_min, current_max};
    return result;
}

/**
 * Creates an array of string values from the given array of integer values.
 *
 * @param arr: Static array of integers
 *
 * returns: Array of strings "fizz", "buzz", or "fizzbuzz" corresponding to
 *          what the integers are divisible by: 3 to "fizz", 5 to "buzz", and
 *          3 and 5 (or 15) to "fizzbuzz"
 */
char **fizz_buzz(int *arr, int len)
{
    char **new_arr = calloc(len, sizeof(char*));
    for (int i = 0; i < len; i++) {
	char *str_val = calloc(12, sizeof(char));
        if (arr[i] % 15 == 0) {
	    strcpy(str_val, "'fizzbuzz'");
        } else if (arr[i] % 3 == 0) {
	    strcpy(str_val, "'fizz'");
        } else if (arr[i] % 5 == 0) {
	    strcpy(str_val, "'buzz'");
        } else if (arr[i] < 0) {
	    snprintf(str_val, 12, "-%d", abs(arr[i]));
        } else {
	    snprintf(str_val, 12, "%d", abs(arr[i]));
        }
        new_arr[i] = str_val;
    }
    return new_arr;
}

void free_fizzbuzz(char **ptr_arr, int len)
{
    for (int i = 0; i < len; i++) {
	free(ptr_arr[i]);
    }
    free(ptr_arr);
}

/**
 * Reverses the order of elements in the given array of integers
 *
 * @param: arr: Array of ints
 *
 * returns: None (array itself modified)
 */
void reverse(int *arr, int len)
{
    int tmp;
    for (int i = 0; i < len / 2; i++) {
        tmp = arr[i];
        arr[i] = arr[len - 1 - i];
	arr[len - 1 - i] = tmp;
    }
}
