/*
	Hash table implementation written in C

	BSD 2-Clause License

	Copyright (c) 2019, Daniel Lorch
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-table.h"

void fatal_error(char* msg) {
	fprintf(stderr, "%s", msg);
	exit(1);
}

hash_table* hash_table_new() {
	// important: memory must be zeroed, since linked list's append() checks for NULL values
	hash_table* table = calloc(1, sizeof(hash_table));

	if(table == NULL) {
		fatal_error("calloc failed\n");
	}

	return table;
}

void hash_table_destroy(hash_table** table) {
	for(int i=0; i<NUM_BUCKETS; i++) {
		hash_table_entry* bucket = (*table)->buckets[i];
		linked_list_destroy(&bucket);
	}

	free(*table);

	*table = NULL;
}

int hashfunc(char* key) {
	int hash = 0;

	for(int i=0; i<strlen(key); i++) {
		hash = hash + (int)key[i];
	}

	return hash;
}

void hash_table_insert(hash_table* table, char* key, char* value) {
	int hash = hashfunc(key);
	int index = hash % NUM_BUCKETS;

	if(table->buckets[index] == (hash_table_entry*)NULL) {
		table->buckets[index] = linked_list_new(key, value);
	} else {
		linked_list_append(table->buckets[index], key, value);
	}
}

void linked_list_append(hash_table_entry* entry, char* key, char* value) {
	hash_table_entry* last = entry;

	// fast-forward to last element
	while(last->next != (hash_table_entry*)NULL) {
		last = last->next;
	}

	last->next = linked_list_new(key, value);
}

hash_table_entry* linked_list_new(char* key, char* value) {
	hash_table_entry* entry = malloc(sizeof(hash_table_entry));

	if(entry == NULL) {
		fatal_error("malloc failed\n");
	}

	entry->next = (hash_table_entry*)NULL;
	entry->key = strdup(key);
	entry->value = strdup(value);

	return entry;
}

void linked_list_destroy(hash_table_entry** entries) {
	hash_table_entry* current = *entries;
	hash_table_entry* next;

	while(current != (hash_table_entry*)NULL) {
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}

	*entries = (hash_table_entry*)NULL;
}

char* hash_table_lookup(hash_table* table, char* key) {
	char* result = (char*)NULL;
	int hash = hashfunc(key);
	int index = hash % NUM_BUCKETS;

	if(table->buckets[index] != (hash_table_entry*)NULL) {
		result = linked_list_lookup(table->buckets[index], key);
	}

	return result;
}

char* linked_list_lookup(hash_table_entry* entries, char* key) {
	char* result = (char*)NULL;
	hash_table_entry* entry = entries;

	while(entry != (hash_table_entry*)NULL) {
		if(strcmp(key, entry->key) == 0) {
			result = entry->value;
			break;
		}
		entry = entry->next;
	}

	return result;
}

// Hash table: https://en.wikipedia.org/wiki/Hash_table
int main() {
	hash_table* phone_book = hash_table_new();

	hash_table_insert(phone_book, "John Smith", "521-1234");
	hash_table_insert(phone_book, "Lisa Smith", "521-8976");
	hash_table_insert(phone_book, "Sandra Dee", "521-9655");
	hash_table_insert(phone_book, "Ted Baker", "418-4165");
	hash_table_insert(phone_book, "Sandra Doe", "521-5030");

	char* key = "Lisa Smith";
	char* value = hash_table_lookup(phone_book, key);

	if(value != (char*)NULL) {
		printf("Phone number of %s: %s.\n", key, value);
	} else {
		printf("Could not find %s in the phone book.\n", key);
	}

	hash_table_destroy(&phone_book);

	return 0;
}