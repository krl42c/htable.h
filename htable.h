#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// FNV-1 hash: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
uint64_t ht_fnv_hash(char *data, size_t size) {
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

typedef struct Item {
    char* key;
    char* value;
} Item;

Item* ht_create_item(char *key, char *value) {
    Item* item = (Item*) malloc(sizeof(Item));
    item->key = (char*) malloc(sizeof(key));
    item->value = (char*) malloc(sizeof(value));

    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

void ht_free_item(Item *item) {
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

void ht_insert(HTable *table, Item *item) {
    uint64_t index = ht_fnv_hash(item->key, table->size);
    if (table->items[index] == NULL) {
        table->items[index] = item;
    } else {
        if (strcmp(table->items[index]->key, item->key) != 0)  {
            // Open-Addressing collision soloving: Find first empty slot
            while (index != table->size) {
                index++;     
                if (table->items[index] == NULL) {
                    table->items[index] = item;
                    break;
                }
            }
        }
    }
}

Item* ht_find_item(HTable *table, char* key) {
    uint64_t index = ht_fnv_hash(key, table->size);
    Item *found = table->items[index];
    
    if (strcmp(found->key, key)) return found;
    
    while (index != table->size) {
        index++;
        if (strcmp(table->items[index]->key, key)) return table->items[index];
    }

    return NULL;
}

char* ht_get(HTable *table, char* key) {
    uint64_t index = ht_fnv_hash(key, table->size);
    return table->items[index]->value;
}

void ht_free_table(HTable *table) {
    free(table->items);
    free(table);
}
