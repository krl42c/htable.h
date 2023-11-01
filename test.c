#include "htable.h"

int main(void) {

    HTable *hash_table = ht_create_hash_table(50); 
    Item *item = ht_create_item("test", "test_value");
    
    ht_insert(hash_table, item);
    
    char* found_item = ht_get(hash_table, "test");
    printf("found item: %s", found_item);

    ht_free_item(item);
    ht_free_table(hash_table);
    return 0;
}
