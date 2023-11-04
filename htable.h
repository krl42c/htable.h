#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// FNV-1 hash: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
static inline uint64_t ht_fnv_hash(char *data, size_t size) {
    uint64_t fnv_offset_basis = 0xcbf29ce484222325;
    uint64_t fnv_prime = 0x100000001b3;

    uint64_t hash = fnv_offset_basis;
    
    char *p = data;
    for (char c = *p; c != '\0'; c = *++p) {
        hash = hash * fnv_prime;
        hash = hash ^ c;
    }

    return hash % size;
}

typedef enum HT_RESULT {
    HT_OK = 0,
    HT_MEM_ERR = 1,
    HT_ERR = 2,
    HT_DUP_KEY = 3
} HT_RESULT; 

typedef struct Item {
    char* key;
    char* value;
} Item;

static Item* ht_create_item(char *key, char *value) {
    Item* item = (Item*) malloc(sizeof(Item));
    item->key = (char*) malloc(strlen(key) + 1);
    item->value = (char*) malloc(strlen(value) + 1);

    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

static void ht_free_item(Item *item) {
    free(item->key);
    free(item->value);
    free(item);
}

typedef struct HTable {
    Item** items;
    size_t size;
    size_t count;
} HTable;

HTable* ht_create_hash_table(size_t size) {
    HTable *table = (HTable*) malloc(sizeof(HTable));
    table->items = (Item**) calloc(size, sizeof(Item)); 
    table->size = size;
    return table;
}

static HT_RESULT ht_insert(HTable *table, Item *item) {
    bool has_inserted = false;
    uint64_t index = ht_fnv_hash(item->key, table->size);
    if (table->items[index] == NULL) {
        table->items[index] = item;
        has_inserted = true;
    } else {
        if (strcmp(table->items[index]->key, item->key) != 0)  {
            // Open-Addressing collision solving: Find first empty slot
            while (index != table->size) {
                index++;     
                if (table->items[index] == NULL) {
                    table->items[index] = item;
                    has_inserted = true;
                    break;
                }
            }
        } else {
            return HT_DUP_KEY;
        }
    }

    if (!has_inserted) return HT_ERR;

    return HT_OK;
}

static Item* ht_find_item(HTable *table, char* key) {
    uint64_t index = ht_fnv_hash(key, table->size);
    Item *found = table->items[index];
    if (found == NULL) return NULL;

    if (strcmp(found->key, key) == 0) return found;
    
    while (index != table->size) {
        index++;
        if (table->items[index] != NULL) { 
            if (strcmp(table->items[index]->key, key) == 0) return table->items[index];
        }
    }

    return NULL;
}

static char* ht_get(HTable *table, char* key) {
    Item *item = ht_find_item(table, key);
    if (item == NULL) return NULL;

    return item->value;
}

HT_RESULT ht_delete(HTable *table, char* key) {
    Item *item = ht_find_item(table, key);
    if (item == NULL) return HT_ERR;

    ht_free_item(item);
    table->items[ht_fnv_hash(key, table->size)] = NULL;
    return HT_OK;
}

static void ht_free_table(HTable *table) {
    free(table->items);
    free(table);
}
