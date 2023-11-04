#include "htable.h"

int main(void) {
    HTable *htable = ht_create_hash_table(10);
    HT_RESULT res = ht_put(htable, "key", "value");

    if (res != HT_OK) printf("Error inserting value\n");

    return 0;
}
