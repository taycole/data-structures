// Name: int_da.c
// Purpose: Implements dynamic arrays for integers
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

// Gets the number of elements in an array
#define NELEMS(arr) (sizeof(arr) / sizeof(*arr))


typedef struct Int_DA {
    size_t size;
    size_t capacity;
    int* data;
} Int_DA;


/**
 * Creates an empty dynamic array on the heap with default capacity 4
 */
Int_DA* da_init(void)
{
    Int_DA* new_arr = malloc(sizeof(Int_DA));
    new_arr->size = 0;
    new_arr->capacity = 4;
    new_arr->data = malloc(new_arr->capacity * sizeof(*new_arr->data));

    return new_arr;
}

void da_free(Int_DA* da_arr)
{
    free(da_arr->data);
    free(da_arr);
}

/**
 * Prints array in a simple way for testing
 */
void da_print(Int_DA* arr)
{
    printf("DA ARR Length: %zu, Capacity: %zu: Data: ", arr->size, arr->capacity);
    for (int i = 0; i < arr->size; i++) {
	printf("%d ", arr->data[i]);
    }
    printf("\n");
}

/**
 * Resize the array when size meets or exceeds current capacity
 */
void da_resize(Int_DA* arr, int new_capacity)
{
    // Must be a positive integer greater than current size
    if (new_capacity <= 0 || new_capacity < arr->size) {
	return;
    }

    arr->data = realloc(arr->data, new_capacity * sizeof(*arr->data));
    arr->capacity = new_capacity;
}

/**
 * Adds a new value at the end of the dynamic array
 */
void da_append(Int_DA* arr, int val)
{
    if (arr->size == arr->capacity)
	da_resize(arr, arr->capacity * 2);

    arr->data[arr->size++] = val;
}

/**
 * Converts a static integer array to a dynamic integer array
 */
Int_DA* da_convert(int* arr, size_t len)
{
    Int_DA* da_arr = da_init();

    // Copy values
    for (int i = 0; i < len; i++) {
	da_append(da_arr, arr[i]);
    }
    return da_arr;
}

/**
 * Inserts a value at the specified index
 */
void da_insert_at_index(Int_DA* arr, int index, int val)
{
    // Bounds check
    if (index < 0 || index > arr->size)
	return;

    // If at capacity
    if (arr->size == arr->capacity)
	da_resize(arr, arr->capacity * 2);

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
void da_remove_at_index(Int_DA* arr, int index)
{
    // Bounds check
    if (index < 0 || index > arr->size)
	return;

    // Shrinks array if size less than 1/4 capacity
    if (arr->capacity / 4 > arr->size) {
        // If size less than 5, defaults to 10 capacity
	if (arr->size < 5 && arr->capacity > 10)
	    da_resize(arr, 10);
	else
	    da_resize(arr, arr->size * 2);
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
Int_DA* da_slice(Int_DA* arr, int start, int size)
{
    // Bounds check
    if ((start < 0 || start >= arr->size) ||
	(size < 0 || size > arr->size - start))
	return NULL;

    Int_DA* new_arr = da_init();

    for (int i = 0; i < size; i++)
	da_insert_at_index(new_arr, i, arr->data[start+i]);

    return new_arr;
}

/**
 * This one slices from between a start and end.
 */
Int_DA* da_slice_sv(Int_DA* arr, int start, int end)
{
    return da_slice(arr, start, end - start + 1);
}

/**
 * Creates an array of values derived from the given function
 */
Int_DA* da_map(Int_DA* arr, int (*func)(int))
{
    Int_DA* new_arr = da_init();
    for (int i = 0; i < arr->size; i++)
	da_append(new_arr, func(arr->data[i]));

    return new_arr;
}

/**
 * Creates a new array of values that pass a filter function
 */
Int_DA* da_filter(Int_DA* arr, bool (*func)(int))
{
    Int_DA* new_arr = da_init();
    for (int i = 0; i < arr->size; i++) {
	if (func(arr->data[i]) == true)
	    da_append(new_arr, arr->data[i]);
    }
    return new_arr;
}

// ------------------- Stack Implementation ----------------------------------

/**
 * Pushes an element onto the stack. Alias for appending.
 */
void da_push(Int_DA* arr, int val)
{
    da_append(arr, val);
}

/**
 * Pops the top element off the stack and returns it (end of dynamic array)
 */
int da_pop(Int_DA* arr)
{
    if (arr->size == 0) {
	perror("da_pop: no elements to pop");
	exit(-1);
    }
    int val = arr->data[arr->size-1];
    da_remove_at_index(arr, arr->size-1);
    
    return val;
}

/**
 * Returns the top value without popping
 */
int da_top(Int_DA* arr)
{
    if (arr->size == 0) {
	perror("da_top: no elements to look at");
	exit(-1);
    }
    return arr->data[arr->size-1];
}

// ------------------- Queue --------------------------------------------------

/**
 * Alias for appending
 */
void da_enqueue(Int_DA* arr, int val)
{
    da_append(arr, val);
}

/**
 * Dequeues value from front of array (index 0)
 */
int da_dequeue(Int_DA* arr)
{
    if (arr->size == 0) {
	perror("da_dequeue: no elements to dequeue");
	exit(-1);
    }
    int val = arr->data[0];
    da_remove_at_index(arr, 0);
    return val;
}

/**
 * Return value at the front of queue without dequeuing
 */
int da_peek(Int_DA* arr)
{
    if (arr->size == 0) {
	perror("da_peek: no elements to peek at");
	exit(-1);
    }
    return arr->data[0];
}
