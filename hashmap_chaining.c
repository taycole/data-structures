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

typedef struct Key {
    const char* data;
    size_t size;
} Key;

typedef struct SLL_Node {
    struct SLL_Node* next;
    Key* key;
    int value;
} SLL_Node;

typedef struct SLL {
    SLL_Node* head;
    size_t size;
} SLL;

typedef struct Buckets {
    size_t size;
    size_t capacity;
    SLL** data;
} Buckets;

typedef struct Map_MC {
    size_t size;
    size_t capacity;
    Buckets* buckets;
    int (*hash_function)(Key*);
} Map_MC;

Key keyify(const char* str);

void sll_insert(SLL* sll, Key* key, int val);
bool sll_remove(SLL* sll, Key* key);
SLL_Node* sll_contains(SLL* sll, Key* key);

Map_MC* map_mc_init(int (*fptr)(Key*));
void map_mc_free(Map_MC* map);
float map_mc_load(Map_MC* map);
void map_mc_resize(Map_MC* map, int new_capacity);
void map_mc_put(Map_MC* map, const char* key_str, int val);
int map_mc_empty_buckets(Map_MC* map);
int map_mc_get(Map_MC* map, const char* key_str);
bool map_mc_contains_key(Map_MC* map, const char* key_str);
void map_mc_test_print(Map_MC* map);

bool is_prime(int num);
int next_prime(int num);
int hash_function_1(Key* key);
int hash_function_2(Key* key);


int main()
{
    Map_MC* test = map_mc_init(hash_function_1);

    // 35
    char* key_list[] = {
	"Lorem",                  // 0
	"ipsum",		  // 1
	"dolor",		  // 2
	"sit",			  // 3
	"amet",			  // 4
	"consectetur",		  // 5
	"adipiscing",		  // 6
	"elit",			  // 7
	"Nam",			  // 8
	"scelerisque",		  // 9
	"purus",		  // 10
	"sollicitudin",		  // 11
	"lacinia",		  // 12
	"ultrices",		  // 13
	"lacus",		  // 14
	"mi",			  // 15
	"finibus",		  // 16
	"ipsum",		  // 17 x2
	"id",			  // 18
	"cursus",		  // 19
	"nunc",			  // 20
	"Lorem",		  // 21 x2
	"et",			  // 22
	"diam",			  // 23
	"Donec",		  // 24
	"nisl",			  // 25
	"libero",		  // 26
	"bibendum",		  // 27
	"sit",			  // 28 x2
	"amet",			  // 29 x2
	"vehicula",		  // 30
	"eget",			  // 31
	"commodo",		  // 32
	"ac",			  // 33
	"sem",			  // 34
    };

    for (int i = 0; i < 35; i++) {
	map_mc_put(test, key_list[i], i);
    }

    map_mc_test_print(test);

    printf("\n\nNumber of empty buckets: %d\n\n", map_mc_empty_buckets(test));

    printf("Get test - key: %s, val: %d\n\n", "Lorem", map_mc_get(test, "Lorem"));
    printf("Map contains key %s: %d\n", "Lorem", map_mc_contains_key(test, "Lorem"));
    printf("Map contains key %s: %d\n", "dog", map_mc_contains_key(test, "dog"));

    map_mc_free(test);

    return 0;
}

/**
 * Convert char* to key
 */
Key keyify(const char* str)
{
    return (Key) {
	.data = str,
	.size = strlen(str),
    };
}



// Functions for the SLL node ------------------------------------------

/**
 * Inserts a new node at the front of the SLL
 */
void sll_insert(SLL* sll, Key* key, int val)
{
    SLL_Node* head = malloc(sizeof(SLL_Node));
    head->key = key;
    head->value = val;
    head->next = sll->head;
    sll->head = head;
    sll->size++;
}

/**
 * Removes the first node with a matching key. Returns true if successful,
 * false otherwise.
 */
bool sll_remove(SLL* sll, Key* key)
{
    SLL_Node* prev = NULL;
    SLL_Node* curr = sll->head;

    while (curr) {
	if (!strcmp(curr->key->data, key->data)) {
	    if (prev)
		prev->next = curr->next;
	    else
		sll->head = curr->next;
	    sll->size--;
	    free(curr);             // Check if this is working, may not free all
	    return true;
	}
	prev = curr;
	curr = curr->next;
    }
    return false;
}

/**
 * Checks if SLL contains a node with the matching key. Returns it or NULL
 */
SLL_Node* sll_contains(SLL* sll, Key* key)
{
    SLL_Node* curr = sll->head;
    while (curr) {
	if (!strcmp(curr->key->data, key->data)) // BUSTED
	    return curr;
	curr = curr->next;
    }
    return curr; // Will return tail NULL
}

// Functions for the hash map ------------------------------------------

/**
 * Initializes map struct with default values
 */
Map_MC* map_mc_init(int (*fptr)(Key*))
{
    Map_MC* map = malloc(sizeof(Map_MC));
    map->buckets = da_init(Buckets);
    map->size = 0;
    map->capacity = 11;
    map->hash_function = fptr;

    for (int i = 0; i < map->capacity; i++) {
	SLL* bucket = malloc(sizeof(SLL));
	bucket->head = NULL;
	bucket->size = 0;
	da_insert_at_index(map->buckets, i, bucket);
    }
    return map;
}

void map_mc_free(Map_MC* map)
{
    da_free(map->buckets);
    free(map);
}

/**
 * Updates or adds a key/value pair to the map
 */
void map_mc_put(Map_MC* map, const char* key_str, int val)
{
    Key* key = malloc(sizeof(Key));
    *key = keyify(key_str);

    // Resizes table if the load factor is overlimit
    if (map_mc_load(map) >= 1.0)
	map_mc_resize(map, map->capacity * 2);

    // Calculates the index and gets the node at the correct bucket
    int hash_val = map->hash_function(key);
    int index = hash_val % map->capacity;
    SLL_Node* bucket_node = sll_contains(map->buckets->data[index], key);

    // If there isn't a node for the given key, add it, otherwise update
    if (bucket_node == NULL) {
	sll_insert(map->buckets->data[index], key, val);
	map->size++;
    }
    else
	bucket_node->value = val;
}

/**
 * Gets the number of empty buckets in the hash map
 */
int map_mc_empty_buckets(Map_MC* map)
{
    int count = 0;
    for (int i = 0; i < map->buckets->size; i++) {
	SLL* bucket = map->buckets->data[i];
	if (bucket->size == 0)
	    count++;
    }
    return count;
}

/**
 * Get the value at the key
 */
int map_mc_get(Map_MC* map, const char* key_str)
{
    Key key = keyify(key_str);

    int hash_val = map->hash_function(&key);
    int index = hash_val % map->capacity;
    SLL_Node* node = sll_contains(map->buckets->data[index], &key);
    if (node)
	return node->value;
    else {
	perror("key not found");
	return -1;
    }
}

/**
 * Checks if the hash map contains a given key
 */
bool map_mc_contains_key(Map_MC* map, const char* key_str)
{
    Key key = keyify(key_str);

    int hash_val = map->hash_function(&key);
    int index = hash_val % map->capacity;
    SLL_Node* node = sll_contains(map->buckets->data[index], &key);
    if (node)
	return true;

    return false;
}

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

    Buckets* old_buckets = map->buckets;
    size_t size = map->size;

    // Set new capacity and reset bucket array and size
    map->capacity = new_capacity;
    map->buckets = da_init(Buckets);
    map->size = 0;

    // Reinitialize bucket array with SLLs
    for (int i = 0; i < map->capacity; i++) {
	SLL* bucket = malloc(sizeof(SLL));
	bucket->head = NULL;
	bucket->size = 0;
	da_insert_at_index(map->buckets, i, bucket);
    }

    // Copy values to new hash map
    int count = 0, bucket_i = 0;
    while (count < size) {
	SLL* bucket_sll = old_buckets->data[bucket_i];
	bucket_i++;

	// For each bucket, iterate through nodes and remap/rehash
	SLL_Node* curr = bucket_sll->head;
	while (curr) {
	    map_mc_put(map, curr->key->data, curr->value);
	    curr = curr->next;
	    count++;
	}
    }
    da_free(old_buckets); // Need to free linked lists as well, rewrite
}

// Functions for testing --------------------------------------------------

/**
 * Prints out the hashmap for testing
 */
void map_mc_test_print(Map_MC* map)
{
    for (int i = 0; i < map->capacity; i++) {
	printf("%d: { ", i);
	SLL_Node* bucket_curr = map->buckets->data[i]->head;
	while (bucket_curr) {
	    printf("%.*s : %d, ",
		   (int)bucket_curr->key->size,
		   bucket_curr->key->data,
		   bucket_curr->value);
	    bucket_curr = bucket_curr->next;
	}
	printf("}\n");
    }
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

/**
 * Basic hash function for testing
 */
int hash_function_1(Key* key)
{
    int hash = 0;
    for (int i = 0; i < key->size; i++) {
	char letter = *(key->data + i);
	hash += (int)letter;
    }
    return hash;
}

/**
 * Second basic hash function for testing
 */
int hash_function_2(Key* key)
{
    int hash = 0;
    for (int i = 0; i < key->size; i++) {
	char letter = *(key->data + i);
	hash += (i + 1) * (int)letter;
    }
    return hash;
}




































