// Name: binary_tree.c
// Purpose: Implements a binary tree data type for integers
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

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




/**
 * Converts a binary tree to a queue linked list via a BFS / level order traversal.
 */
BT_Queue* bt_convert_queue(BT* bt)
{
    BT_Queue* node_store = queue_init();
    BT_Queue* traverse = queue_init();
    Q_Node* curr;
    enqueue(traverse, bt-root);

    while (queue_size(traverse) != 0) {
	curr = dequeue(traverse);
	if (curr != NULL) {
	    enqueue(node_store, curr);
	    enqueue(traverse, curr->left);
	    enqueue(traverse, curr->right);
	}
    }
       
    return q;
}

// ----------------------------------------------------------------------------

/**
 * Recursive helper function to print out a binary tree.
 */
void bt_rec_print_subtree(BT* bt, BT_Node* node)
{
    if (!node) {
	printf("Bottom-------------------");
	return;
    }

    
}

/**
 * prints out the binary tree. Uses a recursive helper function.
 */
void bt_print_tree(BT* bt)
{
    if (bt->root != NULL)
	bt_rec_print_subtree(bt, bt->root);
    else
	printf("(tree is empty)");
}

void print_indent_spaces(int num_spaces, char* str)
{
    printf("%*s%s", num_spaces, "", str);
}

/**
 * prints out the binary tree stack. Preorder (NLR) gives the correct values.
 */
//void bt_print(BT* bt)
//{
//    int num_nodes = sll_length(stack);
//    int height = (int)log2(num_nodes);
//    
//    for(int i = 0; i < len; i++) {
//	
//    }
//}

int main()
{
    BT* test = bt_init();
    int bt_vals[] = {64, 32, 16, 48, 56, 80, 72, 88, 84, 96};

    for (int i = 0; i < 10; i++) bt_add(test, bt_vals[i]);

    Int_Queue* bt_q = bt_convert_queue(test);

    queue_print(bt_q);

    return 0;
}
