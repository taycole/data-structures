// da_macros.c
//
// Implements a dynamic array using macros, allowing it to be generic.
// Not type-safe. May have unforseen consequences.
//
// Create a type like this, then pass the name to da_init()
//
//       typedef struct NAME {
//           size_t size;
//           size_t capacity;
//           TYPE* data;
//       } NAME;
//
// Author: Taylor Cole

#include <stdio.h> // printf
#include <stdlib.h> // malloc, realloc, free, etc.
#include <stdbool.h> // true and false
#include <errno.h> // perror

/**
 * Creates an empty dynamic array on the heap with default capacity 4
 */
#define da_init(Type)\
({\
    Type* new_arr = malloc(sizeof(Type));\
    new_arr->size = 0;\
    new_arr->capacity = 4;\
    new_arr->data = malloc(new_arr->capacity * sizeof(*new_arr->data));\
    new_arr;\
})

#define da_free(arr)\
do {\
    free(arr->data);\
    free(arr);\
} while(0)

/**
 * Resize the array when size meets or exceeds current capacity
 */
#define da_resize(array, new_capacity)\
do {\
    if (new_capacity > 0 && new_capacity >= array->size) {\
	array->data = realloc(array->data, new_capacity * sizeof(*array->data));\
	array->capacity = new_capacity;\
    }\
} while(0)

/**
 * Adds a new value at the end of the dynamic array
 */
#define da_append(array, val)\
do {\
    if (array->size == array->capacity)\
	da_resize(array, array->capacity * 2);\
    array->data[array->size++] = val;\
} while(0)

/**
 * Converts a static array to a dynamic array
 */
// TODO: Rewrite and test using typeof()
#define da_convert(Type, arr, len)\
({\
    Type* da_arr = da_init(Type);\
    for (int i = 0; i < len; i++)\
	da_append(da_arr, arr[i]);\
    da_arr;\
})

/**
 * Inserts a value at the specified index
 */
#define da_insert_at_index(da_arr, index, val)\
do {\
    if (index >= 0 && index <= da_arr->size) {\
	if (da_arr->size == da_arr->capacity)\
	    da_resize(da_arr, da_arr->capacity * 2);\
	for (int i = da_arr->size; i > index; i--) {\
	    da_arr->data[i] = da_arr->data[i-1];\
	}\
	da_arr->data[index] = val;\
	da_arr->size++;\
    }\
} while(0)

/**
 * Remove the value at the specified index
 */
#define da_remove_at_index(arr, index)\
do {\
    if (index >= 0 && index <= arr->size) {\
	if (arr->capacity / 4 > arr->size) {\
	    if (arr->size < 5 && arr->capacity > 10)\
		da_resize(arr, 10);\
	    else\
		da_resize(arr, arr->size * 2);\
	}\
	if (arr->size == 1) {\
	    arr->size--;\
	}\
	else {\
	    for (int i = index; i < arr->size - 1; i++)\
		arr->data[i] = arr->data[i+1];\
	    arr->size--;\
	}\
    }\
} while(0)

/**
 * Swaps two elements in the array given their indices
 */
#define da_swap(arr, index_1, index_2)\
({\
    typeof(*arr) temp = arr->data[index_1];\
    arr->data[index_1] = arr->data[index_2];\
    arr->data[index_2] = temp;\
})

/**
 * Get a slice!! Returns a new array slice of the given size starting at the
 * given index.
 */
#define da_slice(arr, start, slice_size)\
({\
    typeof(arr) new_arr = da_init(typeof(*arr));\
    if ((start < 0 || start >= arr->size) ||\
	(slice_size < 0 || slice_size > arr->size - start))\
	new_arr->data = NULL;\
    else {\
	for (int i = 0; i < slice_size; i++)\
	    da_insert_at_index(new_arr, i, arr->data[start+i]);\
    }\
    new_arr;\
})

/**
 * This one slices from between a start and end.
 */
#define da_slice_sv(arr, start, end)\
    (da_slice(arr, start, end - start + 1))


/**
 * Creates an array of values derived from the given function
 */
#define da_map(arr, func)\
({\
    typeof(arr) new_arr = da_init(typeof(*arr));\
    for (int i = 0; i < arr->size; i++)\
	da_append(new_arr, func(arr->data[i]));\
    new_arr;\
})

/**
 * Creates a new array of values that pass a filter function
 */
#define da_filter(arr, func)\
({\
    typeof(arr) new_arr = da_init(typeof(*arr));\
    for (int i = 0; i < arr->size; i++) {\
	if (func(arr->data[i]) == true)\
	    da_append(new_arr, arr->data[i]);\
    }\
    new_arr;\
})

// ------------------ Stack ---------------------------------------------------

/**
 * Alias for da_append
 */
#define da_push(arr, val) da_append(arr, val)

/**
 * Pops the value off the end of the array and returns it
 */
#define da_pop(arr)\
({\
    if (arr->size == 0) {\
	perror("da_pop: no elements to pop");\
	exit(-1);\
    }\
    typeof(arr->data[0]) val = arr->data[arr->size-1];\
    da_remove_at_index(arr, arr->size-1);\
    val;\
})

/**
 * Returns the top value without popping
 */
#define da_top(arr)\
({\
    if (arr->size == 0) {\
	perror("da_top: no elements to look at");\
	exit(-1);\
    }\
    arr->data[arr->size-1];\
})

// ------------------ Queue ---------------------------------------------------

/**
 * Alias for da_append
 */
#define da_enqueue(arr, val) da_append(arr, val)

/**
 * Dequeues value from front of array (index 0)
 */
#define da_dequeue(arr)\
({\
    if (arr->size == 0) {\
	perror("da_dequeue: no elements to dequeue");\
	exit(-1);\
    }\
    typeof(arr->data[0]) val = arr->data[0];\
    da_remove_at_index(arr, 0);\
    val;\
})

/**
 * Return value at the front of queue without dequeuing
 */
#define da_peek(arr)\
({\
    if (arr->size == 0) {\
	perror("da_peek: no elements to peek at");\
	exit(-1);\
    }\
    arr->data[0];\
})

// TODO -------------------- Min Heap -----------------------------------------


// ------------------------- Print the DA -------------------------------------

/**
 * Can print DA values. Must pass the format specifier for printf.
 */
#define da_print(arr, format_str)\
({\
    printf("( ");\
    for (int i = 0; i < arr->size; i++)\
	printf(format_str, arr->data[i]);\
    printf(")\n");\
})
