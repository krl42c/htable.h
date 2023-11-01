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
        ht_insert(hash_table, garbage);
    }

    Item *item = ht_create_item("test", "test_value");
    
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

    printf("-----------------------------------------------\n");
    printf("Table: " MAG " %zu " RESET " size\n", table_size);
    printf("Time took to insert element: " MAG " %f " RESET "\n", elapsed_time_insert);
    printf("Time took to retrieve element: " MAG " %f " RESET "\n", elapsed_time_retrieve);
    printf("-----------------------------------------------\n");
    printf("Correct lookup test\n");
    printf("Expecting value " MAG "[test_value]" RESET ", found: " GRN "[%s]\n" RESET, found);

    printf("-----------------------------------------------\n");
    printf("Testing collision: 8yn0iYCKYHlIj4-BwPqk=VALUE1 and GReLUrM4wMqfg9yzV3KQ=VALUE2\n");

    Item *col1 = ht_create_item("asdsad", "v1");
    Item *col2 = ht_create_item("8grelurm4wmqfg9yzv3kq", "VALUE2");
    
    ht_insert(hash_table, col2);
    //ht_insert(hash_table, col2);

    char* colide_val = ht_get(hash_table, "8grelurm4wmqfg9yzv3kq");
    printf("8yn0iYCKYHlIj4-BwPqk=%s\n", colide_val);

    printf("-----------------------------------------------\n");
    ht_free_item(item);
    ht_free_table(hash_table);


    return 0;
}
