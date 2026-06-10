// Name: binary_tree.c
// Purpose: Implements a binary tree data type for integers
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include "da_macros.c"

#define LEN(arr) (sizeof(arr) / sizeof(*arr))

#define NLR   0
#define LNR   1
#define LRN   2

typedef struct BT_Node {
    int value;
    struct BT_Node* left;
    struct BT_Node* right;
} BT_Node;

typedef struct BT {
    BT_Node* root;
} BT;

/**
 * Initialize an empty tree
 */
BT* bt_init()
{
    BT* bt = malloc(sizeof(BT));
    bt->root = NULL;
    return bt;
}

/**
 * Create a node with the given value
 */
BT_Node* bt_node_init(int val)
{
    BT_Node* new_node = malloc(sizeof(BT_Node));
    new_node->value = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/**
 * Add a node with a value to the binary tree
 */
void bt_add(BT* bt, int val)
{
    if (bt->root == NULL) {
	bt->root = bt_node_init(val);
	return;
    }

    BT_Node *curr = bt->root, *parent;

    while (curr) {
	parent = curr;
	if (val < curr->value)
	    curr = curr->left;
	else
	    curr = curr->right;
    }

    BT_Node* new_node = bt_node_init(val);

    if (val < parent->value)
	parent->left = new_node;
    else
	parent->right = new_node;
}

/**
 * Removes a value from the tree. Returns true if successful, false otherwise
 */
bool bt_remove(BT* bt, int val);






// ----------- Functions to find height/width, print tree ---------------------

/**
 * Recursive helper function to get the max height of the tree.
 */
void rec_get_height(BT_Node* node, int curr_h, int* h)
{
    if (node != NULL) {
	rec_get_height(node->left, curr_h+1, h);
	rec_get_height(node->right, curr_h+1, h);
        *h = (curr_h > *h)? curr_h : *h;
    }
}

/**
 * Finds the max height of the tree using a recursive DFS traversal
 */
int bt_get_height(BT* bt)
{
    int h = 0;
    rec_get_height(bt->root, 0, &h);
    return h;
}

typedef struct Queue_DA {
    size_t size;
    size_t capacity;
    BT_Node** data;
} Queue_DA;

/**
 * Finds the max width of the tree using a BFS traversal with a queue
 */
int bt_get_width(BT* bt)
{
    int max_w = 1;
    Queue_DA* q = da_init(Queue_DA);
    da_enqueue(q, bt->root);

    while (q->size != 0) {
	max_w = (q->size > max_w)? q->size : max_w;
	BT_Node* node = da_dequeue(q);
	
	if (node->left) da_enqueue(q, node->left);
	if (node->right) da_enqueue(q, node->right);
    }
    return max_w;
}

typedef struct Arr_2D {
    int data[512][512];
} Arr_2D;

/**
 * Puts values of tree into a matrix in order to print. Uses a DFS search.
 */
void rec_fill_matrix(BT_Node* node, Arr_2D* matrix, int row, int l_col, int r_col)
{
    if (node) {
	int mid_col = (l_col + r_col) / 2;
	matrix->data[row][mid_col] = node->value;
	rec_fill_matrix(node->left, matrix, row+1, l_col, mid_col-1);
	rec_fill_matrix(node->right, matrix, row+1, mid_col+1, r_col);
    }
}

/**
 * Prints binary tree by recording it in a matrix. Only looks right if the
 * tree is somewhat balanced due to symmetric spacing.
 */
void bt_print_queue(BT* bt)
{
    // Padding added to keep the symmetry right
    int h = bt_get_height(bt) + 1;
    int w = bt_get_width(bt);
    w = (w % 2 == 0)? (w+1)*3 : w*3;

    Arr_2D* matrix = malloc(sizeof(Arr_2D));
    memset(matrix->data, 0, sizeof(matrix->data));

    rec_fill_matrix(bt->root, matrix, 0, 0, w-1);

    for(int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    if (matrix->data[i][j])
		printf("%d ", matrix->data[i][j]);
	    else
		printf("-- ");
	}
	printf("\n");
    }
    free(matrix);
}



int main()
{
    BT* test = bt_init();
    int bt_vals[] = {64, 32, 16, 48, 56, 80, 72, 88, 84, 96};
    //int bt_vals[] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 5};

    for (int i = 0; i < LEN(bt_vals); i++) bt_add(test, bt_vals[i]);

    //int height = bt_get_height(test);
    //int width = pow(2, height) - 1;
    //printf("Height: %d, Width: %d\n", height, width);

    bt_print_queue(test);

    return 0;
}





///**
// * Recursive helper to push binary tree values onto linked-list stack in
// * order (LNR)
// */
//void rec_convert_stack(BT_Node* node, Int_Node* stack, int order)
//{
//    if (order == NLR) {
//        if (node) {
//            sll_push(stack, node->value);
//	    rec_convert_stack(node->left, stack, order);
//	    rec_convert_stack(node->right, stack, order);
//        }
//    }
//    else if (order == LNR) {
//        if (node) {
//	    rec_convert_stack(node->left, stack, order);
//            sll_push(stack, node->value);
//	    rec_convert_stack(node->right, stack, order);
//        }
//    }
//    else if (order == LRN) {
//        if (node) {
//	    rec_convert_stack(node->left, stack, order);
//	    rec_convert_stack(node->right, stack, order);
//            sll_push(stack, node->value);
//        }
//    }
//    else
//	return;
//}
//
///**
// * Converts a binary tree to a stack as a linked list in the given order:
// *    NLR: Preorder traversal
// *    LNR: Inorder traversal
// *    LRN: Postorder traversal
// */
//Int_Node* bt_convert_stack(BT* bt, int order)
//{
//    Int_Node* stack = sll_init();
//    rec_convert_stack(bt->root, stack, order);
//    return stack;
//}
