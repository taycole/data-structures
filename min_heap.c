// min_heap.c
//
// Implements a Min Heap data structure using dynamic arrays. Allows usual
// add/remove operations and can heapify a given array. Provides a separate
// function to heapsort an array.
//
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // log(), pow()
#include "int_da.c" // Includes Int_DA datatype

typedef struct Arr_2D {
    int data[512][512];
} Arr_2D;

/**
 * Adds a value to the min heap
 */
void heap_add(Int_DA* arr, int val)
{
    int cur_index = arr->size;
    int p_index = (cur_index - 1) / 2;

    // First adds value to the end of the array
    da_append(arr, val);

    // Bubbles up values as needed, swapping with parent
    while (arr->data[p_index] > arr->data[cur_index]) {
	int p_val = arr->data[p_index];
	arr->data[p_index] = val;
	arr->data[cur_index] = p_val;

	cur_index = p_index;
	p_index = (cur_index - 1) / 2;
    }
}

/**
 * Helper function to percolate a value down the array from a given start point
 * to the end of the given length.
 */
void heap_percolate_down(Int_DA* arr, int p_index, int length)
{
    int left_index = 2 * p_index + 1;
    int right_index = 2 * p_index + 2;

    // Continues to the end so long as one of the child indices points to a
    // valid position and the child value is less than the parent
    while ((left_index < length && arr->data[p_index] > arr->data[left_index]) ||
	   (right_index < length && arr->data[p_index] > arr->data[right_index])) {

	// If only one child, go that way, otherwise smallest child
	int min_child;
	if (right_index >= length)
	    min_child = left_index;
	else if (left_index >= length)
	    min_child = right_index;
	else if (arr->data[left_index] <= arr->data[right_index])
	    min_child = left_index;
	else
	    min_child = right_index;

	// Swaps parent value with the smaller child's value
	int p_val = arr->data[p_index];
	arr->data[p_index] = arr->data[min_child];
	arr->data[min_child] = p_val;

	// Updates variables for next iteration from child's old position
	p_index = min_child;
        left_index = 2 * p_index + 1;
        right_index = 2 * p_index + 2;
    }
}

/**
 * Removes the minimum value from the heap and returns it.
 */
int heap_remove_min(Int_DA* arr)
{
    if (arr->size == 0) {
	perror("heap_remove_min: array empty");
	exit(-1);
    }

    int min = arr->data[0];

    // Replaces root with value at end of array then removes it
    arr->data[0] = arr->data[arr->size - 1];
    da_remove_at_index(arr, arr->size - 1);

    // Percolate down if array isn't empty
    if (arr->size != 0)
	heap_percolate_down(arr, 0, arr->size);

    return min;
}

/**
 * Heapifies an unsorted array into a proper heap in place.
 */
void heapify(Int_DA* arr)
{
    // Start from first non-leaf element and move up the tree, percolating down
    // as needed
    for (int cur_index = arr->size / 2 - 1; cur_index >= 0; cur_index--)
	heap_percolate_down(arr, cur_index, arr->size);
}

/**
 * Heapsorts an array in place using the heapsort algorithm.
 */
void heapsort(Int_DA* arr)
{
    // First puts array in heap order
    heapify(arr);

    // Swaps last element with the first and percolates down to heap length
    for (int heap_len = arr->size; heap_len > 1; heap_len--) {
	int root_val = arr->data[0];
	arr->data[0] = arr->data[heap_len-1];
	arr->data[heap_len-1] = root_val;
	heap_percolate_down(arr, 0, heap_len-1);
    }
}


/**
 * Puts keys of tree into a matrix in order to print. Uses a DFS search. Modified
 * for array structure.
 */
void rec_fill_matrix(Int_DA* node, int index, Arr_2D* matrix, int row, int l_col, int r_col)
{
    if (index < node->size) {
	int mid_col = (l_col + r_col) / 2;
	matrix->data[row][mid_col] = node->data[index];
	rec_fill_matrix(node, (2*index + 1), matrix, row+1, l_col, mid_col-1);
	rec_fill_matrix(node, (2*index + 2), matrix, row+1, mid_col+1, r_col);
    }
}


/**
 * Prints the heap dynamic array as a tree
 */
void heap_print(Int_DA* arr)
{
    // calculate max height from number of nodes
    int h = (int)(log2f((float)arr->size + 1.0) + 1.0);

    // Padding added to keep the symmetry somewhat right
    int w = (pow(2, h-1) + 2) * 2;

    // Fill matrix
    Arr_2D matrix = {0};
    rec_fill_matrix(arr, 0, &matrix, 0, 0, w-1);

    // Print matrix
    for(int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    if (matrix.data[i][j])
		printf("%02d ", matrix.data[i][j]);
	    else
		printf("-- ");
	}
	printf("\n");
    }
}



int main()
{
    Int_DA* test = da_init();
    for (int i = 14; i > 0; i--) heap_add(test, i);
    da_print(test);
    printf("----");

    heapsort(test);
    da_print(test);

    return 0;
}
