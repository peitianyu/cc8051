#include "hash_map.h"

static unsigned int hash_function(const char* str) {
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash << 5) + hash + str[i];
    }
    return hash % HASH_MAP_SIZE;
}

static void free_node(MapNode* node) {
    if (node) {
        free(node->key);
        free(node);
    }
}

void hash_map_init(HashMap* map) {
    memset(map->buckets, 0, sizeof(map->buckets));
}

HashMap create_hash_map()
{
    HashMap map;
    hash_map_init(&map);
    return map;
}

void hash_map_insert(HashMap* map, const char* key, void* value) {
    unsigned int index = hash_function(key);
    MapNode* new_node = (MapNode*)malloc(sizeof(MapNode));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        return;
    }
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = map->buckets[index]; 
    map->buckets[index] = new_node;
}

void* hash_map_get(HashMap* map, const char* key) {
    unsigned int index = hash_function(key);
    MapNode* node = map->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL; 
}

void hash_map_set(HashMap* map, const char* key, void* value) {
    unsigned int index = hash_function(key);
    MapNode* node = map->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return;
        }
        node = node->next;
    }
}

void hash_map_erase(HashMap* map, const char* key) {
    unsigned int index = hash_function(key);
    MapNode* node = map->buckets[index];
    MapNode* prev = NULL;
    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                map->buckets[index] = node->next;
            }
            free_node(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

void hash_map_traverse(HashMap* map, void (*callback)(const char*, void*)) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        MapNode* node = map->buckets[i];
        while (node) {
            callback(node->key, node->value);
            node = node->next;
        }
    }
}

int hash_map_size(HashMap* map) {
    int count = 0;
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        MapNode* node = map->buckets[i];
        while (node) {
            count++;
            node = node->next;
        }
    }
    return count;
}

void hash_map_clear(HashMap* map) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        MapNode* node = map->buckets[i];
        while (node) {
            MapNode* temp = node;
            node = node->next;
            free_node(temp);
        }
    }
    memset(map->buckets, 0, sizeof(map->buckets));
}


