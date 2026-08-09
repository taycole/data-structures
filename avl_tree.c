// avl_tree.c
//
// Implements an avl tree data structure. Builds off the previous binary tree
// implementation.
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

typedef struct AVL_Node {
    int key;
    int height;
    struct AVL_Node* left;
    struct AVL_Node* right;
    struct AVL_Node* parent;
} AVL_Node;

typedef struct AVL {
    AVL_Node* root;
} AVL;

typedef struct AVL_Queue_DA {
    size_t size;
    size_t capacity;
    AVL_Node** data;
} AVL_Queue_DA;

typedef struct Int_Queue_DA {
    size_t size;
    size_t capacity;
    int* data;
} Int_Queue_DA;

typedef struct Arr_2D {
    int data[512][512];
} Arr_2D;

typedef struct Inorder_S {
    AVL_Node* parent;
    AVL_Node* successor;
} Inorder_S;

/**
 * Initialize an empty tree
 */
AVL* avl_init()
{
    AVL* avl = malloc(sizeof(AVL));
    avl->root = NULL;
    return avl;
}

/**
 * Initialize a node with the given key
 */
AVL_Node* avl_node_init(int val)
{
    AVL_Node* new_node = malloc(sizeof(AVL_Node));
    new_node->key = val;
    new_node->height = 0;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    return new_node;
}

/**
 * Returns the balance factor of the given node.
 */
int avl_balance_factor(AVL_Node* node)
{
    int left_h = (node->left)? node->left->height : -1;
    int right_h = (node->right)? node->right->height : -1;

    return right_h - left_h;
}

/**
 * Updates the height of the node.
 */
void avl_update_height(AVL_Node* node)
{
    if (node == NULL) return;
    int left_h = (node->left)? node->left->height : -1;
    int right_h = (node->right)? node->right->height : -1;
    int max = left_h > right_h ? left_h : right_h;
    node->height = max + 1;
}

/**
 * Rotates a node to the left and returns the promoted child
 */
AVL_Node* avl_rotate_left(AVL_Node* node)
{
    // First reassigns right-child's left to node's right
    AVL_Node* child = node->right;
    node->right = child->left;

    // If it's really a node, update the parent pointer
    if (node->right) node->right->parent = node;

    // node gets set to child's left
    child->left = node;
    node->parent = child;

    // Update the heights and returns promoted child
    avl_update_height(node);
    avl_update_height(child);
    return child;
}

/**
 * Rotates a node to the right and returns the promoted child
 */
AVL_Node* avl_rotate_right(AVL_Node* node)
{
    // First reassigns left-child's right to node's left
    AVL_Node* child = node->left;
    node->left = child->right;

    // Update parent pointer if it's really a node
    if (node->left) node->left->parent = node;

    // Node set to child's right
    child->right = node;
    node->parent = child;

    // Update heights and returns promoted child
    avl_update_height(node);
    avl_update_height(child);
    return child;
}

/**
 * Rebalances the (sub)tree at the given node based on its balance factor.
 */
void avl_rebalance(AVL* avl, AVL_Node* node)
{
    AVL_Node* old_parent = node->parent;

    // Left-heavy, -2
    if (avl_balance_factor(node) < -1) {
        // L-R case - one rotation left at child, then right rotation
        if (avl_balance_factor(node->left) > 0) {
            node->left = avl_rotate_left(node->left);
            node->left->parent = node;
        }
        // L-L case - one rotation to the right
        AVL_Node* new_subtree_root = avl_rotate_right(node);
        new_subtree_root->parent = old_parent;

        // Updates the original parent to point to the new subtree node
        if (old_parent == NULL)
            avl->root = new_subtree_root;
        else if (node->key > old_parent->key)
            old_parent->right = new_subtree_root;
        else
            old_parent->left = new_subtree_root;
    }
    // Right-heavy, 2
    else if (avl_balance_factor(node) > 1) {
        // R-L case - one rotation right at child, then left rotation
        if (avl_balance_factor(node->right) < 0) {
            node->right = avl_rotate_right(node->right);
            node->right->parent = node;
        }
        // R-R case - one rotation to the left
        AVL_Node* new_subtree_root = avl_rotate_left(node);
        new_subtree_root->parent = old_parent;

        // Updates the original parent to point to the new subtree node
        if (old_parent == NULL)
            avl->root = new_subtree_root;
        else if (node->key > old_parent->key)
            old_parent->right = new_subtree_root;
        else
            old_parent->left = new_subtree_root;
    }
    // Nodes are balanced, only update the height
    else
        avl_update_height(node);
}

/**
 * Add a node with a key to the avl tree. Balances the tree afterwards.
 */
void avl_add(AVL* avl, int val)
{
    if (avl->root == NULL) {
        avl->root = avl_node_init(val);
        return;
    }
    // Searches until it find the right parent with a None node
    AVL_Node *curr = avl->root, *parent = NULL;
    while (curr) {
        parent = curr;
        if (val < curr->key)
            curr = curr->left;
        else if (val > curr->key)
            curr = curr->right;
        else
            return;    // Excludes duplicate keys
    }
    // Creates a new node, makes it the child of the parent, updates pointers
    AVL_Node* new_node = avl_node_init(val);
    if (val < parent->key)
        parent->left = new_node;
    else
        parent->right = new_node;

    new_node->parent = parent;

    // Move up the tree, check balance factor, rebalance if needed
    curr = parent;
    while (curr) {
        avl_rebalance(avl, curr);
        curr = curr->parent;
    }
}

/**
 * Removes a node that has no children
 */
void avl_remove_no_subtrees(AVL* avl, AVL_Node* r_parent, AVL_Node* r_node)
{
    // Is the root with no children
    if (r_parent == NULL)
        avl->root = NULL;
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
void avl_remove_one_subtree(AVL* avl, AVL_Node* r_parent, AVL_Node* r_node)
{
    // Determine which child
    AVL_Node* child;
    if (r_node->left == NULL)
        child = r_node->right;
    else
        child = r_node->left;

    // Replace node to be removed with its child
    if (r_parent == NULL)
        avl->root = child;
    else if (r_node->key < r_parent->key)
        r_parent->left = child;
    else
        r_parent->right = child;

    free(r_node);
}

/**
 * Finds the inorder successor and its parent. Returns them together in a struct
 */
Inorder_S avl_find_inorder_successor(AVL_Node* node)
{
    AVL_Node* successor_parent = node;
    AVL_Node* successor = node->right;

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
AVL_Node* avl_remove_two_subtrees(AVL* avl, AVL_Node* r_parent, AVL_Node* r_node)
{
    // Gets inorder successor and its parent from helper function
    Inorder_S s = avl_find_inorder_successor(r_node);

    // Lowest modifies node for rebalancing. Changes if the sucessor's parent
    // is the one to be removed
    AVL_Node* lowest_mod_node = NULL;
    if (s.parent == r_node)
        lowest_mod_node = s.successor;
    else
        lowest_mod_node = s.parent;

    // Updates all the pointers to replace value with in-order successor
    s.successor->left = r_node->left;
    if (r_node->left)
        r_node->left->parent = s.successor;

    if (s.successor != r_node->right) {
        s.parent->left = s.successor->right;
        s.successor->right = r_node->right;

        // Updates the parents is they are nodes
        if (s.parent->left) s.parent->left->parent = s.parent;
        if (s.successor->right) s.successor->right->parent = s.successor;
    }
    // Update removed node's parent to point to new node. If root, set as root.
    if (r_parent == NULL)
        avl->root = s.successor;
    else if (r_node->key < r_parent->key)
        r_parent->left = s.successor;
    else
        r_parent->right = s.successor;

    // Update successor to point to parent and return lowest modified node
    s.parent = r_parent;
    free(r_node);
    return lowest_mod_node;
}

/**
 * Removes a node from the tree with the given key value. Returns true if
 * successful, false otherwise.
 */
bool avl_remove(AVL* avl, int val)
{
    // Searches for the value, ends loop if we hit NULL or the val
    AVL_Node *parent = NULL, *curr_node = avl->root, *start_point = NULL;
    while (curr_node && curr_node->key != val) {
        parent = curr_node;
        if (val < curr_node->key)
            curr_node = curr_node->left;
        else
            curr_node = curr_node->right;
    }
    // Search failed
    if (curr_node == NULL) return false;

    // Node has both children, calls helper function to remove two subtrees
    // TODO: rewrite to use start point
    else if (curr_node->left != NULL && curr_node->right != NULL) {
        start_point = avl_remove_two_subtrees(avl, parent, curr_node);
    }
    // Node has one child, call helper to remove one subtree
    else if (curr_node->left != NULL || curr_node->right != NULL) {
        avl_remove_one_subtree(avl, parent, curr_node);
        start_point = parent;
    }
    // Node is a leaf, call helper to remove with no subtrees
    else {
        avl_remove_no_subtrees(avl, parent, curr_node);
        start_point = parent;
    }

    // Rebalance the tree
    while (start_point) {
        avl_rebalance(avl, start_point);
        start_point = start_point->parent;
    }

    return true;
}

/**
 * Searches for a node in the tree with the given key. Returns true if found,
 * false otherwise.
 */
bool avl_contains(AVL* avl, int key)
{
    AVL_Node* curr = avl->root;
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
void avl_rec_traverse(AVL_Node* node, int mode, Int_Queue_DA* q)
{
    if (mode == NLR) {
        if (node) {
            da_enqueue(q, node->key);
            avl_rec_traverse(node->left, mode, q);
            avl_rec_traverse(node->right, mode, q);
        }
    }
    else if (mode == LNR) {
        if (node) {
            avl_rec_traverse(node->left, mode, q);
            da_enqueue(q, node->key);
            avl_rec_traverse(node->right, mode, q);
        }
    }
    else if (mode == LRN) {
        if (node) {
            avl_rec_traverse(node->left, mode, q);
            avl_rec_traverse(node->right, mode, q);
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
Int_Queue_DA* avl_recursive_traversal(AVL* avl, int mode)
{
    Int_Queue_DA* q = da_init(Int_Queue_DA);
    avl_rec_traverse(avl->root, mode, q);
    return q;
}

/**
 * Performs an iterative traversal of the tree, storing the keys in a queue.
 *
 */
Int_Queue_DA* avl_iterative_traversal(AVL* avl, int mode)
{
    Int_Queue_DA* arr = da_init(Int_Queue_DA);
    AVL_Queue_DA* stack = da_init(AVL_Queue_DA);
    AVL_Node *curr, *prev;

    // Preorder
    if (mode == NLR) {
        if (avl->root) da_push(stack, avl->root);
        while (stack->size != 0) {
            curr = da_pop(stack);
            da_enqueue(arr, curr->key);  // record node - inorder
            if (curr->right) da_push(stack, curr->right);
            if (curr->left) da_push(stack, curr->left);
        }
    }
    // Inorder
    else if (mode == LNR) {
        curr = avl->root;
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
    // TODO: refactor
    // Post Order
    else if (mode == LRN) {
        curr = avl->root;
        prev = NULL;
        while (stack->size != 0 || curr != NULL) {
            if (curr) {
                da_push(stack, curr);
                curr = curr->left;
            }
            // Hit the end of left, try to go right
            else {
                AVL_Node* top = da_top(stack);
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
void avl_free(AVL* avl)
{
    AVL_Node* curr = NULL;
    AVL_Queue_DA* stack = da_init(AVL_Queue_DA);

    if (avl->root) da_push(stack, avl->root);

    while (stack->size != 0) {
        curr = da_pop(stack);
        if (curr->right) da_push(stack, curr->right);
        if (curr->left) da_push(stack, curr->left);
        free(curr);
    }
    da_free(stack);
    free(avl);
}

/**
 * Puts keys of tree into a matrix in order to print. Uses a DFS search.
 */
void rec_fill_matrix(AVL_Node* node, Arr_2D* matrix, int row, int l_col, int r_col)
{
    if (node) {
        int mid_col = (l_col + r_col) / 2;
        matrix->data[row][mid_col] = node->key;
        rec_fill_matrix(node->left, matrix, row+1, l_col, mid_col-1);
        rec_fill_matrix(node->right, matrix, row+1, mid_col+1, r_col);
    }
}

/**
 * Prints avl tree by recording it in a matrix then printing the matrix in
 * a way that looks symmetrical. Can get weird with deep trees
 *
 */
void avl_print(AVL* avl)
{
    int h = avl->root->height;

    // Padding added to keep the symmetry somewhat right
    int w = (pow(2, h-1) + 2) * 3;

    Arr_2D matrix = {0};
    rec_fill_matrix(avl->root, &matrix, 0, 0, w-1);

    for(int i = 0; i < h + 1; i++) {
        for (int j = 0; j < w; j++) {
            if (matrix.data[i][j])
                printf("%02d ", matrix.data[i][j]);
            else
                printf("-- ");
        }
        printf("\n");
    }
}

// ----------------------------------------------------------------------------

int main()
{
    AVL* test = avl_init();
    //int avl_vals[] = {64, 32, 16, 48, 56, 80, 72, 88, 84, 96};
    //int avl_vals[] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 5};
    //int avl_vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //for (int i = 0; i < LEN(avl_vals); i++) avl_add(test, avl_vals[i]);

    for (int i = 1; i < 30; i++) {
        printf("adding %d-----\n", i);
        avl_add(test, i);
        avl_print(test);
        printf("\n-------\n\n");
    }
    //for (int i = 1; i < 12; i++) avl_add(test, i);
    //avl_print_queue(test);
    //printf("\n-------\n\n");



    avl_free(test);
    return 0;
}
