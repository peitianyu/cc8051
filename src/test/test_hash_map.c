#include "common/c_test.h"
#include "common/logger.h"
#include "core/utils/hash_map.h"

void print_map(const char* key, void* value) {
    printf("Key: %s, Value: %s\n", key, (char *)value);
}

// JUST_RUN_TEST(hash_map, test)
TEST(hash_map, test)
{
    HashMap map = create_hash_map();

    printf("=============insert map===========\n");
    hash_map_insert(&map, "key1", (void*)"value1");
    hash_map_insert(&map, "key2", (void*)"value2");
    hash_map_insert(&map, "key3", (void*)"value3");
    hash_map_traverse(&map, print_map);

    printf("=============get map===========\n");
    void* val = hash_map_get(&map, "key1");
    printf("key1: %s\n", (char*)val);

    printf("=============set map===========\n");
    hash_map_set(&map, "key1", (void*)"value4");
    hash_map_traverse(&map, print_map);

    printf("=============erase map===========\n");
    hash_map_erase(&map, "key1");
    hash_map_traverse(&map, print_map);

    printf("=============size map===========\n");
    printf("size: %d\n", (int)hash_map_size(&map));

    printf("=============clear map===========\n");
    hash_map_clear(&map);
    printf("size: %d\n", (int)hash_map_size(&map)); 
}
