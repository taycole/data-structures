/* Static array fundamentals.
   Implements 10 functions that manipulate an array.
 */
#include <stdio.h>

// Gets the number of elements in an array
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

void print_int_array(int *arr, int len);
struct min_max { int min, max; };
struct min_max get_min_max(int *arr, int len);


// Used to test out the functions
int main()
{
    int arr_1[] = {7, 8, 6, -5, 4};
    int len_arr_1 = NELEMS(arr_1);
    printf("min_max example 1:\n");
    printf("Array: ");
    print_int_array(arr_1, len_arr_1);
    struct min_max result = get_min_max(arr_1, len_arr_1);
    printf("Min: %d, Max: %d", result.min, result.max);

    
    
    return 0;
}

// Prints each element of an integer array
void print_int_array(int *arr, int len)
{
    for (int i = 0; i < len; i++) {
	printf("%d ", *(arr + i));
    }
    printf("\n");
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
