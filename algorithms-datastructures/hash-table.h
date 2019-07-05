#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define NUM_BUCKETS 16

/* Misc */

void fatal_error(char* msg);

/* Linked List */

typedef struct _hash_table_entry {
    struct _hash_table_entry* next;
    char* key;
    char* value;
} hash_table_entry;

void linked_list_append(hash_table_entry* entry, char* key, char* value);
hash_table_entry* linked_list_new(char* key, char* value);
void linked_list_destroy(hash_table_entry** list);
char* linked_list_lookup(hash_table_entry* entries, char* key);

/* Hash Table */

typedef struct _hash_table {
    hash_table_entry* buckets[NUM_BUCKETS];
} hash_table;

hash_table* hash_table_new();
void hash_table_destroy(hash_table** table);
void hash_table_insert(hash_table* table, char* key, char* value);
char* hash_table_lookup(hash_table* table, char* key);

/* Hash Function */

int hashfunc(char* key);

#endif