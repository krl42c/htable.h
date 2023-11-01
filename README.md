# htable.h

A simple hash table single header implementation in C.

The hash function used to calculate index hashes is [FNV-1 hash](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)

Hash collisions are solved through [Open Addressing](https://en.wikipedia.org/wiki/Open_addressing)

---

Example usage:


```c
HTable *hash_table = ht_create_hash_table(50); 
Item *item = ht_create_item("test", "test_value");

ht_insert(hash_table, item);

char* found_item = ht_get(hash_table, "test");
printf("found item: %s", found_item);
```
