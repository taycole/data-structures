// string_da.c
//
// Implements dynamic arrays for a string datatype.
//
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

// Gets the number of elements in an array
#define NELEMS(arr) (sizeof(arr) / sizeof(*arr))


typedef struct Str_DA {
    size_t size;
    size_t capacity;
    char* data;
} Str_DA;


/**
 * Creates an empty dynamic array on the heap with default capacity 4
 */
Str_DA* str_da_init(void)
{
    Str_DA* new_arr = malloc(sizeof(Str_DA));
    new_arr->size = 0;
    new_arr->capacity = 4;
    new_arr->data = malloc(new_arr->capacity * sizeof(*new_arr->data));

    return new_arr;
}

void str_da_free(Str_DA* str_da_arr)
{
    free(str_da_arr->data);
    free(str_da_arr);
}

/**
 * Prints array in a simple way for testing
 */
void str_da_test_print(Str_DA* arr)
{
    printf("DA ARR Length: %zu, Capacity: %zu ", arr->size, arr->capacity);
    printf("Data: %s", arr->data);
    printf("\n");
}

/**
 * Shorthand to print the array without having to remember %.*s
 */
void str_da_print(Str_DA* arr)
{
    printf("%.*s", (int)arr->size, arr->data);
}

/**
 * Resize the array when size meets or exceeds current capacity
 */
void str_da_resize(Str_DA* arr, int new_capacity)
{
    // Must be a positive integer greater than current size
    if (new_capacity <= 0 || new_capacity < arr->size) {
	return;
    }

    arr->data = realloc(arr->data, new_capacity * sizeof(*arr->data));
    arr->capacity = new_capacity;
}

/**
 * Adds a new character at the end of the dynamic array
 */
void str_da_append(Str_DA* arr, char val)
{
    if (arr->size == arr->capacity)
	str_da_resize(arr, arr->capacity * 2);

    arr->data[arr->size++] = val;
}

/**
 * Initialized a dynamic char array using a given static char array and len
 */
Str_DA* str_da_set(char* arr, size_t len)
{
    Str_DA* str_da_arr = str_da_init();

    // Copy values
    for (int i = 0; i < len; i++) {
	str_da_append(str_da_arr, arr[i]);
    }
    return str_da_arr;
}

/**
 * Inserts a value at the specified index
 */
void str_da_insert_at_index(Str_DA* arr, int index, char val)
{
    // Bounds check
    if (index < 0 || index > arr->size)
	return;

    // If at capacity
    if (arr->size == arr->capacity)
	str_da_resize(arr, arr->capacity * 2);

    // Shift values to the right at insert location
    for (int i = arr->size; i > index; i--) {
	arr->data[i] = arr->data[i-1];
    }
    arr->data[index] = val;
    arr->size++;
}

/**
 * Remove the value at the specified index
 */
void str_da_remove_at_index(Str_DA* arr, int index)
{
    // Bounds check
    if (index < 0 || index > arr->size)
	return;

    // Shrinks array if size less than 1/4 capacity
    if (arr->capacity / 4 > arr->size) {
	// If size less than 5, defaults to 10 capacity
	if (arr->size < 5 && arr->capacity > 10)
	    str_da_resize(arr, 10);
	else
	    str_da_resize(arr, arr->size * 2);
    }
    // If only one element, remove it and return
    if (arr->size == 1) {
	arr->size--;
	return;
    }

    // Remove value by shifting to the left and overriding
    for (int i = index; i < arr->size - 1; i++)
	arr->data[i] = arr->data[i+1];

    arr->size--;
}

/**
 * Get a slice!! Returns a new array slice of the given size starting at the
 * given index.
 */
Str_DA* str_da_slice(Str_DA* arr, int start, int size)
{
    // Bounds check
    if ((start < 0 || start >= arr->size) ||
	(size < 0 || size > arr->size - start))
	return NULL;

    Str_DA* new_arr = str_da_init();

    for (int i = 0; i < size; i++)
	str_da_insert_at_index(new_arr, i, arr->data[start+i]);

    return new_arr;
}

/**
 * This one slices from between a start and end.
 */
Str_DA* str_da_slice_sv(Str_DA* arr, int start, int end)
{
    return str_da_slice(arr, start, end - start + 1);
}

/**
 * Concatenates src to dest string and returns the pointer to the dest
 */
Str_DA* str_da_cat(Str_DA* dest, const Str_DA* src)
{
    for(int i = 0; i < src->size; i++) {
	str_da_append(dest, src->data[i]);
    }
    return dest;
}

Str_DA* str_da_dup(const Str_DA* src)
{
    Str_DA* new_str = str_da_init();

    for(int i = 0; i < src->size; i++) {
	str_da_append(new_str, src->data[i]);
    }
    return new_str;
}

bool str_da_cmp(const Str_DA* str_1, const Str_DA* str_2)
{
    if (str_1->size != str_2->size)
	return false;

    for (int i = 0; i < str_1->size; i++) {
	if (str_1->data[i] != str_2->data[i])
	    return false;
    }
    return true;
}


// Test stuff
/*

int main()
{
    char* msg1 = "This is a test!";
    Str_DA* test = str_da_set(msg1, strlen(msg1));

    char* msg2 = " Is it working???";
    Str_DA* test2 = str_da_set(msg2, strlen(msg2));

    str_da_cat(test, test2);

    Str_DA* test3 = str_da_dup(test);

    str_da_print(test);

    printf("\nIs this the same?:\n\n");

    str_da_print(test3);

    printf("\nAre they equal? %s", str_da_cmp(test, test3) ? "true" : "false");

    return 0;
}
*/
