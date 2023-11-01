#include "htable.h"

int main(void) {

    HTable *hash_table = create_hash_table(50); 
    Item *item = create_item("test", "test_value");
    
    insert(hash_table, item);
    
    char* found_item = get(hash_table, "test");
    printf("found item: %s", found_item);

    free_item(item);
    free(hash_table);
    return 0;
}
