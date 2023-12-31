#include <time.h>

#include "htable.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int main(void) {
    size_t table_size = 100;
    float start_time, end_time;

    HTable *hash_table = ht_create_hash_table(table_size); 
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("test_keys.txt", "r");
    while ((read = getline(&line, &len, fp) != -1)) {
        Item *garbage = ht_create_item(line, "val");
        HT_RESULT res = ht_insert(hash_table, garbage);
        if (res == HT_DUP_KEY ) {
            printf("Failed to insert record (duplicate key): %s", garbage->key);
        }
        if (res == HT_ERR || res == HT_MEM_ERR) {
            printf("Failed to insert record (unknown error): %s", garbage->key);
        }
    }

    Item *item = ht_create_item("test", "test_value");
    Item *item2 = ht_create_item("test_2", "test_value_2");
    Item *item3 = ht_create_item("test_3", "test_value_3");
    Item *item4 = ht_create_item("test_4", "test_value_4");
    Item *item5 = ht_create_item("test_5", "test_value_5");

    // Insert
    start_time = (float) clock() / CLOCKS_PER_SEC;
    ht_insert(hash_table, item);
    end_time = (float) clock() / CLOCKS_PER_SEC;
    float elapsed_time_insert = end_time - start_time; 

    // Get
    start_time = (float) clock() / CLOCKS_PER_SEC;
    char* found = ht_get(hash_table, "test");
    end_time = (float) clock() / CLOCKS_PER_SEC;

    float elapsed_time_retrieve = end_time - start_time;

    ht_insert(hash_table, item2);
    ht_insert(hash_table, item3);
    ht_insert(hash_table, item4);
    ht_insert(hash_table, item5);

    printf("-----------------------------------------------\n");
    printf("Table: " MAG " %zu " RESET " size\n", table_size);
    printf("Time took to insert element: " MAG " %f " RESET "\n", elapsed_time_insert);
    printf("Time took to retrieve element: " MAG " %f " RESET "\n", elapsed_time_retrieve);
    printf("-----------------------------------------------\n");

    printf("Correct lookup test\n");
    printf("Expecting value " MAG "[test_value]" RESET ", found: " GRN "[%s]\n" RESET, found);
    printf("Expecting value " MAG "[test_value_2]" RESET ", found: " GRN "[%s]\n" RESET, ht_get(hash_table,"test_2"));
    printf("Expecting value " MAG "[test_value_3]" RESET ", found: " GRN "[%s]\n" RESET, ht_get(hash_table,"test_3"));
    printf("Expecting value " MAG "[test_value_4]" RESET ", found: " GRN "[%s]\n" RESET, ht_get(hash_table,"test_4"));
    printf("Expecting value " MAG "[test_value_5]" RESET ", found: " GRN "[%s]\n" RESET, ht_get(hash_table,"test_5"));

    printf("-----------------------------------------------\n");
    printf("Testing collision: gMPflVXtwGDXbIhP73TX and LtHf1prlU1bCeYZEdqWf\n");

    Item *col1 = ht_create_item("gMPflVXtwGDXbIhP73TX", "COL 1");
    Item *col2 = ht_create_item("LtHf1prlU1bCeYZEdqWf", "COL 2");
    
    ht_insert(hash_table, col1);
    ht_insert(hash_table, col2);

    char* colide_val1 = ht_get(hash_table, "gMPflVXtwGDXbIhP73TX");
    char* colide_val2 = ht_get(hash_table, "LtHf1prlU1bCeYZEdqWf");

    char* colide_val1_again = ht_get(hash_table, "gMPflVXtwGDXbIhP73TX");

    printf("Expected value: " MAG " [%s] " RESET ", got "GRN" [%s] " RESET "\n", "COL 1", colide_val1);
    printf("Expected value: " MAG " [%s]" RESET ", got "GRN" [%s] " RESET "\n", "COL 2", colide_val2);
    //printf("Expected value: " MAG " [%s] " RESET ", got "GRN" [%s] " RESET "\n", "COL 1", colide_val1_again);

    printf("-----------------------------------------------\n");
    printf("Delete test\n");

    ht_delete(hash_table, "gMPflVXtwGDXbIhP73TX");

    char* deleted = ht_get(hash_table, "gMPflVXtwGDXbIhP73TX");
    if (deleted == NULL) printf("Element deleted correctly\n");

    printf("-----------------------------------------------\n");
    printf("Update test\n");

    ht_insert(hash_table, ht_create_item("to_update", "before update"));

    ht_update(hash_table, "to_update", "after update");

    printf("Expected value: " MAG " [after update] " RESET ", got "GRN" [%s] " RESET "\n", ht_get(hash_table, "to_update"));

    ht_free_table(hash_table);

    return 0;
}
