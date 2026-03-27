/* Static array fundamentals.
   Implements 10 functions that manipulate an array.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
int *rotate(int *arr, int len, int steps);
int *sa_range(int start, int end);
int is_int_sorted(int *arr, int len);

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
    printf("\nrotate example 1\n");
    int rotate_test[] = {-20, -13, -6, 1, 8, 15};
    print_int_array(rotate_test, 6);
    int steps[] = {1, 2, 0, -1, -2, 28, -100, pow(2, 28), -pow(2, 28)};
    
    for (int i = 0; i < 9; i++) {
        int *rotated_arr = rotate(rotate_test, 6, steps[i]);
	printf("Steps: %d ", steps[i]);
        print_int_array(rotated_arr, 6);
	free(rotated_arr);
    }
    print_int_array(rotate_test, 6);
    // -----------------------------------------------------------------------
    // sa_range tests
    printf("\nsa_range tests\n");
    int cases[] = {1, 3, -1, 2, 0, 0, 0, -3, -95, -89, -89, -95};
    for (int i = 0; i < 12; i += 2) {
	printf("%d to %d:  ", cases[i], cases[i+1]);
        int *sa_arr = sa_range(cases[i], cases[i+1]);
        print_int_array(sa_arr, (abs(cases[i] - cases[i + 1]) + 1));
	free(sa_arr);
    }
    // -----------------------------------------------------------------------
    // is_int_sorted
    printf("\nis_int_sorted tests\n");
    int sort_test1[] = {-100, -8, 0, 2, 3, 10, 20, 100};
    int sort_test2[] = {1, 3, -10, 20, -30, 0};
    int sort_test3[] = {-10, 0, 0, 10, 20, 30};
    int sort_test4[] = {100, 90, 0, -90, -200};
    int sort_test5[] = {35730,  77423, 66302,  -10656, 44764,
                        -81911, 19699, -89130, -45360};
    int str_sort_test1[] = {'A', 'B', 'Z', 'a', 'z'};
    int str_sort_test2[] = {'Z', 'T', 'K', 'A', '5'};
    int str_sort_test3[] = {'a', 'p', 'p', 'l', 'e'};
    int *sort_test_cases[] = {sort_test1,     sort_test2,    sort_test3,
                              sort_test4,     sort_test5,    str_sort_test1,
                              str_sort_test2, str_sort_test3};
    int sort_test_lens[] = {8, 6, 6, 5, 9, 5, 5, 5, 5};

    printf("problem child: ");
    print_int_array(str_sort_test2, 5);
    printf("  Result: %d\n\n", is_int_sorted(str_sort_test2, 5));

    
    int i = 0;
    for (i = 0; i < 8; i++) {
        printf("Checking: ");
        print_int_array(sort_test_cases[i], sort_test_lens[i]);
        printf("  Result: %d",
               is_int_sorted(sort_test_cases[i], sort_test_lens[i]));
	printf("\n");
    }

    
    
    
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

/**
 * Shifts values of the given integer array a number of steps left or right
 *
 * @param arr: Array of ints to be rotated
 * @param len: Length of the given array
 * @param steps: Positive or negative int
 *
 * returns: New array containing the rotated values.
 */
int *rotate(int *arr, int len, int steps)
{
    int *new_arr = calloc(len, sizeof(int));
    
    // First adjusts number of steps to its smaller equivalent
    // 14 steps equivalent to 2 steps for array length 6, etc.
    int adj_steps = abs(steps) % len;

    // Converts negative steps to positive equivalent (2 left = 4 right)
    if (steps < 0)
        adj_steps = len - adj_steps;

    for (int i = 0; i < len; i++)
	new_arr[((i + adj_steps) % len)] = arr[i];

    return new_arr;
}

/**
 * Creates an array containing all values between two integers (inclusive)
 *
 * @param start: Int start point
 * @param end:   Int end point
 *
 * returns: Int array containing the range of values
 */
int *sa_range(int start, int end)
{
    // Negative increment if end < start
    int n = (end < start) ? -1 : 1;

    // Initializes array with start and end values
    int arr_len = (abs(start - end) + 1);
    int *arr = calloc(arr_len, sizeof(int));
    arr[0] = start;
    arr[arr_len - 1] = end;

    // Fills in the middle values
    for (int i = 1; i < arr_len - 1; i++) {
	arr[i] = start + n * i;
    }
    return arr;
}

/**
 * Checks if the given integer array is strictly ascending, descending or
 * neither.
 *
 * @param arr: Array of integers
 * @param len: Length of array
 *
 * returns: 1 if strictly ascending, -1 for descending, 0 for neither
 */
int is_int_sorted(int *arr, int len)
{
    // One element considered strictly ascending
    if (len == 1)
        return 1;
    // If start and end equal, must be neither
    if (arr[0] == arr[len - 1])
        return 0;
    // Array order indicators. 1 for ascending, -1 for descending
    int arr_order = (arr[0] < arr[len - 1]) ? 1 : -1;

    // Check remaining values between first and last index
    for (int i = 1; i < len / 2; i++) {
        // Adjacent values equal? return 0
        if (arr[i - 1] == arr[i] || arr[len - i] == arr[len - 1 - i])
            return 0;

	// This shit is fucked-------------------------------
        // Breaks loop if expected order incorrent
        if ((arr_order == 1 && !(arr[i - 1] < arr[i] < arr[len - 1 - i])) ||
            (arr_order == -1 && !(arr[i - 1] > arr[i] > arr[len - 1 - i])))
	    return 0;
    }
    return arr_order;
}
