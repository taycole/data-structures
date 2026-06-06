// Name: int_sll.c
// Purpose: Implements a singly linked list data structure for integers
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Int_Node {
    int value;
    struct Int_Node* next;
} Int_Node;

typedef struct Int_SLL {
    Int_Node* head;
} Int_SLL;

/**
 * Creates a node on the heap with the given value
 */
Int_Node* sll_new_node(int val)
{
    Int_Node* node = malloc(sizeof(Int_Node));
    node->value = val;
    node->next = NULL;
    return node;
}

/**
 * Initializes a singly linked list. The head is an empty node
 */
Int_SLL* sll_init(void)
{
    Int_SLL* sll = malloc(sizeof(Int_SLL));
    Int_Node* head = malloc(sizeof(Int_Node));
    head->next = NULL;
    sll->head = head;
    return sll;
}

/**
 * Frees all the nodes of the linked list
 */
void sll_free(Int_SLL* sll)
{
    Int_Node* node = sll->head;
    Int_Node* next_node;

    while (node) {
	next_node = node->next;
	free(node);
	node = next_node;
    }
    free(sll);
}

/**
 * Get the length of the linked list
 */
int sll_length(Int_SLL* sll)
{
    int len = 0;
    Int_Node* node = sll->head->next;
    while (node) {
	len++;
	node = node->next;
    }
    return len;
}

/**
 * Inserts a node at the beginning of the list
 */
void sll_insert_front(Int_SLL* sll, int val)
{
    Int_Node* new_node = sll_new_node(val);
    new_node->next = sll->head->next;
    sll->head->next = new_node;
}

/**
 * Inserts a node at the end of the list
 */
void sll_insert_back(Int_SLL* sll, int val)
{
    Int_Node* new_tail = sll_new_node(val);
    Int_Node* curr = sll->head;

    while (curr->next) curr = curr->next;

    curr->next = new_tail;
}

/**
 * Inserts a node at the given index
 */
int sll_insert_at_index(Int_SLL* sll, int index, int val)
{
    if (index < 0) {
	perror("index out of bounds");
	return 1;
    }
    Int_Node* curr = sll->head;
    for (int i = 0; i < index; i++) {
	curr = curr->next;
	if (curr == NULL) {
	    perror("sll not long enough");
	    return 1;
	}
    }
    Int_Node* new_node = sll_new_node(val);
    new_node->next = curr->next;
    curr->next = new_node;

    return 0;
}

/**
 * Removes node at the specified index
 */
int sll_remove_at_index(Int_SLL* sll, int index)
{
    if (index < 0 || sll->head->next == NULL) {
	perror("list not long enough");
	return 1;
    }
    Int_Node* curr = sll->head;
    for (int i = 0; i < index; i++) {
	curr = curr->next;
	if (curr->next == NULL) {
	    perror("index not found");
	    return 1;
	}
    }
    // Cut off the node
    curr->next = curr->next->next;
    return 0;
}

/**
 * Removes a node with the matching value
 */
int sll_remove(Int_SLL* sll, int val)
{
    Int_Node* curr = sll->head;

    while (curr->next) {
	if (curr->next->value == val) {
	    curr->next = curr->next->next;
	    return 0;
	}
	curr = curr->next;
    }
    return 1;
}

/**
 * Counts the number of a given value in the list and returns the count
 */
int sll_count(Int_SLL* sll, int val)
{
    int count = 0;

    Int_Node* curr = sll->head->next;

    while (curr) {
	if (curr->value == val)
	    count++;
	curr = curr->next;
    }
    return count;
}


/**
 * Checks if a value is in the list. Returns true or false
 */
bool sll_find(Int_SLL* sll, int val)
{
    Int_Node* curr = sll->head->next;

    while (curr) {
	if (curr->value == val)
	    return true;
	curr = curr->next;
    }
    return false;
}

/**
 * Creates a slice of linked list at start of size
 */
Int_SLL* sll_slice(Int_SLL* sll, int start, int size)
{
    if (start < 0 || size < 0) {
	perror("no");
	return NULL;
    }
    Int_Node* curr = sll->head;

    // Step to start point
    for(int i = 0; i < start + 1; i++) {
	curr = curr->next;
	if (curr == NULL) {
	    perror("no");
	    return NULL;
	}
    }

    Int_SLL* new_list = sll_init();
    Int_Node* new_list_node = new_list->head;

    for (int i = 0; i < size; i++) {
	if (curr == NULL) {
	    perror("out of bounds");
	    return NULL;
	}
	new_list_node->next = sll_new_node(curr->value);
	new_list_node = new_list_node->next;
	curr = curr->next;

    }
    return new_list;
}

/**
 * Prints the linked list
 */
void sll_print(Int_SLL* sll)
{
    Int_Node* curr = sll->head;
    printf("SLL: HEAD ");

    while (curr->next) {
	printf("-> N(%d) ", curr->next->value);
	curr = curr->next;
    }
}
