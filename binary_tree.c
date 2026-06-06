// Name: binary_tree.c
// Purpose: Implements a binary tree data type for integers
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct BT_Node {
    int value;
    struct *BT_Node left;
    struct *BT_Node right;
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

    BT_Node* curr = bt->root, parent;
    
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
