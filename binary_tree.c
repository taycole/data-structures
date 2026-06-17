// binary_tree.c
//
// Implements a binary tree data structure.
//
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
    int key;
    struct BT_Node* left;
    struct BT_Node* right;
} BT_Node;

typedef struct BT {
    BT_Node* root;
} BT;

typedef struct BT_Queue_DA {
    size_t size;
    size_t capacity;
    BT_Node** data;
} BT_Queue_DA;

typedef struct Int_Queue_DA {
    size_t size;
    size_t capacity;
    int* data;
} Int_Queue_DA;

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
 * Create a node with the given key
 */
BT_Node* bt_node_init(int val)
{
    BT_Node* new_node = malloc(sizeof(BT_Node));
    new_node->key = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/**
 * Add a node with a key to the binary tree
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
	if (val < curr->key)
	    curr = curr->left;
	else
	    curr = curr->right;
    }

    BT_Node* new_node = bt_node_init(val);

    if (val < parent->key)
	parent->left = new_node;
    else
	parent->right = new_node;
}

/**
 * Removes a node that has no children
 */
void bt_remove_no_subtrees(BT* bt, BT_Node* r_parent, BT_Node* r_node)
{
    // Is the root with no children
    if (r_parent == NULL)
	bt->root = NULL;
    // Remove node is to the left
    else if (r_node->key < r_parent->key)
	r_parent->left = NULL;
    // Remove node is to the right
    else
	r_parent->right = NULL;

    free(r_node);
}

/**
 * Removes a node from the tree that has one child
 */
void bt_remove_one_subtree(BT* bt, BT_Node* r_parent, BT_Node* r_node)
{
    // Determine which child
    BT_Node* child;
    if (r_node->left == NULL)
	child = r_node->right;
    else
	child = r_node->left;

    // Replace node to be removed with its child
    if (r_parent == NULL)
	bt->root = child;
    else if (r_node->key < r_parent->key)
	r_parent->left = child;
    else
	r_parent->right = child;

    free(r_node);
}

typedef struct Inorder_S {
    BT_Node* parent;
    BT_Node* successor;
} Inorder_S;

/**
 * Finds the inorder successor and its parent. Returns them together in a struct
 */
Inorder_S bt_find_inorder_successor(BT_Node* node)
{
    BT_Node* successor_parent = node;
    BT_Node* successor = node->right;

    // Iterate until the leftmost node
    while (successor->left != NULL) {
	successor_parent = successor;
	successor = successor->left;
    }
    return (Inorder_S){.parent = successor_parent, .successor = successor};
}

/**
 * Removes a node with two children by replacing it with its inorder successor
 */
void bt_remove_two_subtrees(BT* bt, BT_Node* r_parent, BT_Node* r_node)
{
    // Gets inorder successor and its parent from helper function
    Inorder_S s = bt_find_inorder_successor(r_node);

    s.successor->left = r_node->left;
    if (s.successor != r_node->right) {
	s.parent->left = s.successor->right;
	s.successor->right = r_node->right;
    }
    // Update removed node's parent to point to new node. If root, set as root.
    if (r_parent == NULL)
	bt->root = s.successor;
    else if (r_node->key < r_parent->key)
	r_parent->left = s.successor;
    else
	r_parent->right = s.successor;

    free(r_node);
}

/**
 * Removes a key from the tree. Returns true if successful, false otherwise
 */
bool bt_remove(BT* bt, int val)
{
    BT_Node *parent = NULL, *curr_node = bt->root;
    while (curr_node && curr_node->key != val) {
	parent = curr_node;
	if (val < curr_node->key)
	    curr_node = curr_node->left;
	else
	    curr_node = curr_node->right;
    }
    // Search failed
    if (curr_node == NULL)
	return false;

    // Node has both children, calls helper function to remove two subtrees
    else if (curr_node->left != NULL && curr_node->right != NULL)
	bt_remove_two_subtrees(bt, parent, curr_node);
    // Node has one child, call helper to remove one subtree
    else if (curr_node->left != NULL || curr_node->right != NULL)
	bt_remove_one_subtree(bt, parent, curr_node);
    // Node is a leaf, call helper to remove with no subtrees
    else
	bt_remove_no_subtrees(bt, parent, curr_node);

    return true;
}

/**
 * Searches for a node in the tree with the given key. Returns true if found,
 * false otherwise.
 */
bool bt_contains(BT* bt, int key)
{
    BT_Node* curr = bt->root;
    while (curr) {
	if (key == curr->key)
	    return true;
	else if (key < curr->key)
	    curr = curr->left;
	else
	    curr = curr->right;
    }
    return false;
}

/**
 * Recursive helper function to traverse a tree and store keys in a queue.
 */
void bt_rec_traverse(BT_Node* node, int mode, Int_Queue_DA* q)
{
    if (mode == NLR) {
	if (node) {
	    da_enqueue(q, node->key);
	    bt_rec_traverse(node->left, mode, q);
	    bt_rec_traverse(node->right, mode, q);
	}
    }
    else if (mode == LNR) {
	if (node) {
	    bt_rec_traverse(node->left, mode, q);
	    da_enqueue(q, node->key);
	    bt_rec_traverse(node->right, mode, q);
	}
    }
    else if (mode == LRN) {
	if (node) {
	    bt_rec_traverse(node->left, mode, q);
	    bt_rec_traverse(node->right, mode, q);
            da_enqueue(q, node->key);
	}
    }
}

/**
 * Performs a recursive traversal of the tree, stores keys in a queue and
 * returns the queue. Can perform preorder, inorder, or postorder traversal
 * based on specified option. Options have macro names to help.
 *
 * @param: int mode: 0 = NLR = preorder
 *                   1 = LNR = inorder
 *                   2 = LRN = postorder
 *
 */
Int_Queue_DA* bt_recursive_traversal(BT* bt, int mode)
{
    Int_Queue_DA* q = da_init(Int_Queue_DA);
    bt_rec_traverse(bt->root, mode, q);
    return q;
}

/**
 * Performs an iterative traversal of the tree, storing the keys in a queue.
 *
 */
Int_Queue_DA* bt_iterative_traversal(BT* bt, int mode)
{
    Int_Queue_DA* arr = da_init(Int_Queue_DA);
    BT_Queue_DA* stack = da_init(BT_Queue_DA);
    BT_Node *curr, *prev;

    // Preorder
    if (mode == NLR) {
	if (bt->root) da_push(stack, bt->root);
	while (stack->size != 0) {
	    curr = da_pop(stack);
	    da_enqueue(arr, curr->key);  // record node - inorder
    	    if (curr->right) da_push(stack, curr->right);
	    if (curr->left) da_push(stack, curr->left);
	}
    }
    // Inorder
    else if (mode == LNR) {
        curr = bt->root;
	while (stack->size != 0 || curr != NULL) {
	    if (curr) {
		da_push(stack, curr);
		curr = curr->left;
	    }
	    // Enqueue on backtrack
	    else {
	        curr = da_pop(stack);
		da_enqueue(arr, curr->key);
		curr = curr->right;
	    }
	}
    }
    // Post Order
    else if (mode == LRN) {
        curr = bt->root;
	prev = NULL;
	while (stack->size != 0 || curr != NULL) {
	    if (curr) {
		da_push(stack, curr);
		curr = curr->left;
	    }
	    // Hit the end of left, try to go right
	    else {
		BT_Node* top = da_top(stack);
		if (top->right != NULL && top->right != prev) {
		    curr = top->right;
		}
		// Nothing to the right, visit then continue trying right from last
		else {
		    da_enqueue(arr, top->key);
		    prev = top;
		    da_pop(stack);
		}
	    }
	}
    }
    da_free(stack);
    return arr;
}

/**
 * Frees memory for all the nodes then the tree struct itself.
 */
void bt_free(BT* bt)
{
    BT_Node* curr;
    BT_Queue_DA* stack = da_init(BT_Queue_DA);

    if (bt->root) da_push(stack, bt->root);

    while (stack->size != 0) {
	curr = da_pop(stack);
	if (curr->right) da_push(stack, curr->right);
	if (curr->left) da_push(stack, curr->left);
	free(curr);
    }
    da_free(stack);
    free(bt);
}


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

/**
 * Finds the max width of the tree using a BFS traversal with a queue
 */
int bt_get_width(BT* bt)
{
    int max_w = 1;
    BT_Queue_DA* q = da_init(BT_Queue_DA);
    da_enqueue(q, bt->root);

    while (q->size != 0) {
	max_w = (q->size > max_w)? q->size : max_w;
	BT_Node* node = da_dequeue(q);

	if (node->left) da_enqueue(q, node->left);
	if (node->right) da_enqueue(q, node->right);
    }
    da_free(q);
    return max_w;
}

typedef struct Arr_2D {
    int data[512][512];
} Arr_2D;

/**
 * Puts keys of tree into a matrix in order to print. Uses a DFS search.
 */
void rec_fill_matrix(BT_Node* node, Arr_2D* matrix, int row, int l_col, int r_col)
{
    if (node) {
	int mid_col = (l_col + r_col) / 2;
	matrix->data[row][mid_col] = node->key;
	rec_fill_matrix(node->left, matrix, row+1, l_col, mid_col-1);
	rec_fill_matrix(node->right, matrix, row+1, mid_col+1, r_col);
    }
}

/**
 * Prints binary tree by recording it in a matrix then printing the matrix in
 * a way that looks symmetrical. Can get weird with unbalanced trees.
 *
 */
void bt_print(BT* bt)
{
    // Padding added to keep the symmetry right
    int h = bt_get_height(bt) + 1;
    int w = bt_get_width(bt) + 2;
    w = (w % 2 == 0)? (w+1)*3 : w*3;

    Arr_2D* matrix = malloc(sizeof(Arr_2D));
    memset(matrix->data, 0, sizeof(matrix->data));

    rec_fill_matrix(bt->root, matrix, 0, 0, w-1);

    for(int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    if (matrix->data[i][j])
		printf("%02d ", matrix->data[i][j]);
	    else
		printf("-- ");
	}
	printf("\n");
    }
    free(matrix);
}

// ----------------------------------------------------------------------------

int main()
{
    BT* test = bt_init();
    //int bt_vals[] = {64, 32, 16, 48, 56, 80, 72, 88, 84, 96};
    //int bt_vals[] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 5};
    int bt_vals[] = {5, 4, 6, 3, 7, 2, 8};

    for (int i = 0; i < LEN(bt_vals); i++) bt_add(test, bt_vals[i]);

    bt_print(test);
    printf("\n-------\n\n");

    bt_free(test);
    return 0;
}
