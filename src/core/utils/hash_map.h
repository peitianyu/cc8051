#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MAP_SIZE 1021 

typedef struct MapNode {
    char* key;
    void* value; 
    struct MapNode* next; 
} MapNode;

typedef struct HashMap {
    MapNode* buckets[HASH_MAP_SIZE]; 
} HashMap;

HashMap create_hash_map();
void hash_map_init(HashMap* map);
void hash_map_insert(HashMap* map, const char* key, void* value);
void* hash_map_get(HashMap* map, const char* key);
void hash_map_set(HashMap* map, const char* key, void* value);
void hash_map_erase(HashMap* map, const char* key);
void hash_map_traverse(HashMap* map, void (*callback)(const char*, void*));
int hash_map_size(HashMap* map);
void hash_map_clear(HashMap* map);


#endif // __HASH_MAP_H__