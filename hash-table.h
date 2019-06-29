#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define NUM_BUCKETS 16

typedef struct _hash_table_entry {
    struct _hash_table_entry* next;
    char* key;
    char* value;
} hash_table_entry;

typedef struct _hash_table {
    hash_table_entry* buckets[NUM_BUCKETS];
} hash_table;

void fatal_error(char* msg);
hash_table* make_hash_table();
void destroy_hash_table(hash_table** table);
int hashfunc(char* key);
void insert(hash_table* table, char* key, char* value);
void append(hash_table_entry* entry, char* key, char* value);
hash_table_entry* make_linked_list(char* key, char* value);
void destroy_linked_list(hash_table_entry** list);
char* lookup(hash_table* table, char* key);

#endif