// hashmap_chaining.c
//
// Implements a Hash Map that uses linked list chains for collisions. Uses a
// dynamic array for the table and linked lists for the buckets.
//
// Author: Taylor Cole
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "da_macros.c"
#include "generic_sll.c"


typedef struct SLL_Node {
    struct SLL_Node* next;
    char* key;
    int value;
} SLL_Node;

typedef struct Buckets {
    size_t size;
    size_t capacity;
    SLL_Node* data;
} Buckets;

typedef struct Map_MC {
    size_t size;
    size_t capacity;
    Buckets* buckets;
    int (*hash_function)(char*, int);
} Map_MC;


bool is_prime(int num);
int next_prime(int num);
Map_MC* map_mc_init(int (*fptr)(int));
void map_mc_free(Map_MC* map);
float map_mc_load(Map_MC* map);
void map_mc_resize(Map_MC* map, int new_capacity);
void map_mc_put(char* key, int key_len, Map_MC* map);
int hash_function_1(char* s, int len);
int hash_function_2(char* s, int len);


int main()
{
    char* test = "This is a test";
    int result = hash_function_2(test, strlen(test));
    printf("%d", result);
    return 0;
}


/**
 * Tests if a given number is a prime number
 */
bool is_prime(int num)
{
    if (num == 2 || num == 3)
	return true;
    if (num == 1 || num % 2 == 0)
	return false;

    int f = 3;
    while (pow(f, 2) <= num) {
	if (num % f == 0)
	    return false;
	f += 2;
    }
    return true;
}

/**
 * Returns a prime number greater or equal to the given number
 */
int next_prime(int num)
{
    if (num % 2 == 0)
	num += 1;

    while (!is_prime(num))
	num += 2;

    return num;
}

Map_MC* map_mc_init(int (*fptr)(char*, int))
{
    Map_MC* map = malloc(sizeof(Map_MC));
    map->buckets = da_init(Buckets);
    map->size = 0;
    map->capacity = 11;
    map->hash_function = fptr;
    return map;
}

void map_mc_free(Map_MC* map)
{
    da_free(map->buckets);
    free(map);
}


// TODO finish this ----------------------------------------------------

/**
 * Updates or adds a key/value pair to the map
 */
void map_mc_put(char* key, int key_len, int val, Map_MC* map)
{
    // Resizes table if the load factor is overlimit
    if (map_mc_load(map) >= 1.0)
	map_mc_resize(map, map->capacity * 2);

    // Calculates the index and gets the node at the correct bucket
    int hash_val = map->hash_function(key, key_len);
    int index = hash_val % map->capacity;
    
}

bool contains_key(Map_MC* map, char* key, int key_len);

//----------------------------------------------------------------------

/**
 * Calculates and returns the load of the table
 */
float map_mc_load(Map_MC* map)
{
    return (float)map->size / (float)map->capacity;
}

/**
 * Increase capacity of the underlying array and rehash the table
 */
void map_mc_resize(Map_MC* map, int new_capacity)
{
    if (new_capacity < 1)
	return;

    // Capacity not prime? find next closest prime
    if (!is_prime(new_capacity))
	new_capacity = next_prime(new_capacity);

    Buckets* old_bucket = map->buckets;
    size_t size = map->size;

    // Set new capacity and reset bucket array and size
    map->capacity = new_capacity;
    map->buckets = da_init(Buckets);
    map->size = 0;

    // Copy values to new hash map
    int count = 0, bucket_i = 0;
    while (count < size) {
	Buckets* bucket = old_bucket[bucket_i];
	bucket_i++;

	// For each bucket, iterate through nodes and remap/rehash
	SLL_Node* curr = bucket->data;
	while (curr) {
	    map_mc_put(curr->key, curr->value);
	    curr = curr->next;
	    count++;
	}
    }
}

/**
 * Basic hash function for testing
 */
int hash_function_1(char* s, int len)
{
    int hash = 0;
    for (int i = 0; i < len; i++) {
	char letter = *(s + i);
	hash += (int)letter;
    }
    return hash;
}

/**
 * Basic hash function for testing
 */
int hash_function_2(char* s, int len)
{
    int hash = 0;
    for (int i = 0; i < len; i++) {
	char letter = *(s + i);
	hash += (i + 1) * (int)letter;
    }
    return hash;
}


