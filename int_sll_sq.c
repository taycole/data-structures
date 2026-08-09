// int_sll_sq.c
//
// Implements a linked list with head and tail pointers for stacks and queues.
//
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct Int_Node {
    int value;
    struct Int_Node* next;
} Int_Node;

typedef struct Int_SLL {
    Int_Node* head;
    Int_Node* tail;
} Int_SLL;

/**
 * Initialize an empty sll
 */
Int_SLL* sll_init()
{
    Int_SLL* q = malloc(sizeof(Int_SLL));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void sll_free(Int_SLL* q)
{
    Int_Node* curr = q->head, next_node;
    while (curr) {
        next_node = curr->next;
        free(curr);
        curr = next_node;
    }
    free(q);
}

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
 * Returns the size of the sll
 */
int sll_size(Int_SLL* q)
{
    Int_Node* node = q->head;
    int len = 0;

    while (node) {
        len++;
        node = node->next;
    }
    return len;
}

// --------------------- Stack functions --------------------------------------

/**
 * Pushes a value at the beginning of the linked list
 */
void sll_push(Int_SLL* sll, int val)
{
    Int_Node* new_node = sll_new_node(val);
    new_node->next = sll->head;
    sll->head = new_node;
}

/**
 * Pops the value off the beginning of the linked list and returns it
 */
int sll_pop(Int_SLL* sll)
{
    Int_Node* node = sll->head;
    int val = node->value;
    sll->head = sll->head->next;
    free(node);
    return val;
}

/**
 * Returns value at the beginning of the linked list without removing
 */
int sll_peek(Int_SLL* sll)
{
    if (sll->head == NULL) {
        perror("sll empty");
        return -1;
    }
    return sll->head->value;
}

// ------------------------ Queue functions -----------------------------------


/**
 * Adds a value to the end of the linked list
 */
void enqueue(Int_SLL* q, int val)
{
    if (q->tail == NULL) {
        q->head = sll_new_node(val);
        q->tail = q->head;
    }
    else {
        q->tail->next = sll_new_node(val);
        q->tail = q->tail->next;
    }
}

/**
 * Removes and returns the value at the beginning of the linked list
 */
int dequeue(Int_SLL* q)
{
    if (q->head == NULL) {
        perror("queue empty");
        return -1;
    }
    int val = q->head->value;
    q->head = q->head->next;
    return val;
}


// ----------------------------------------------------------------------------


void sll_print(Int_SLL* q)
{
    Int_Node* curr = q->head;
    while (curr) {
        printf("Q(%d) -> ", curr->value);
        curr = curr->next;
    }
}
