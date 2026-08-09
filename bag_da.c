// Name: bag_da.c
// Purpose: Implements a Bag data type using dynamic integer arrays
// Author: Taylor Cole
#include "int_da.c"

/**
 * Adds an element to a bag
 */
void bag_add(Int_DA* bag, int val)
{
    da_append(bag, val);
}

/**
 * Removes an element from the bag. Returns true if successful, false otherwise.
 */
bool bag_remove(Int_DA* bag, int val)
{
    for (int i = 0; i < bag->size; i++) {
        if (bag->data[i] == val) {
            da_remove_at_index(bag, i);
            return true;
        }
    }
    return false;
}

/**
 * Counts the number of a specific element in a bag. Returns the count.
 */
int count(Int_DA* bag, int val)
{
    int val_count = 0;
    for (int i = 0; i < bag->size; i++) {
        if (bag->data[i] == val)
            val_count++;
    }
    return val_count;
}

/**
 * Clears all elements from a bag by reinitializing the internal array
 */
void clear(Int_DA* bag)
{
    free(bag->data);
    bag->size = 0;
    bag->capacity = 4;
    bag->data = malloc(bag->capacity * sizeof(*bag->data));
}

/**
 * Checks if two bags are equal
 */
bool equal(Int_DA* bag_1, Int_DA* bag_2)
{
    if (bag_1->size != bag_2->size)
        return false;

    if (bag_1->size == 0 && bag_2->size == 0)
        return true;

    for (int i = 0; i < bag_1->size; i++) {
        int count_1 = count(bag_1, bag_1->data[i]);
        int count_2 = count(bag_2, bag_2->data[i]);
        if (count_1 != count_2)
            return false;
    }
    return true;
}
