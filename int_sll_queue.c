// Name: int_sll_queue.c
// Purpose: Implements a queue as a singly linked list
// Author: Taylor Cole

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct Int_Node {
    int value;
    struct Int_Node* next;
} Int_Node;

typedef struct Int_Queue {
    Int_Node* head;
    Int_Node* tail;
} Int_Queue;

/**
 * Initialize an empty queue
 */
Int_Queue* queue_init()
{
    Int_Queue* q = malloc(sizeof(Int_Queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
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
 * Returns the size of the queue
 */
int queue_size(Int_Queue* q)
{
    Int_Node* node = q->head;
    int len = 0;

    while (node) {
	len++;
	node = node->next;
    }
    return len;
}

/**
 * Adds a value to the end of the queue
 */
void enqueue(Int_Queue* q, int val)
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
 * Removes and returns the value at the beginning of the queue
 */
int dequeue(Int_Queue* q)
{
    if (q->head == NULL) {
	perror("queue empty");
	return -1;
    }
    int val = q->head->value;
    q->head = q->head->next;
    return val;
}

/**
 * Returns value at the beginning of the queue without removing
 */
int queue_peek(Int_Queue* q)
{
    if (q->head == NULL) {
	perror("queue empty");
	return -1;
    }
    return q->head->value;
}

void queue_print(Int_Queue* q)
{
    Int_Node* curr = q->head;
    while (curr) {
	printf("Q(%d) -> ", curr->value);
	curr = curr->next;
    }
}
