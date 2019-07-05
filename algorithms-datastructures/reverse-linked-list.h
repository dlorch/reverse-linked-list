#ifndef REVERSE_LINKED_LIST_H
#define REVERSE_LINKED_LIST_H

typedef struct _linked_list {
    struct _linked_list* next;
    int value;
} linked_list;

void print_linked_list(linked_list* list);
linked_list* make_linked_list(int value);
void destroy_linked_list(linked_list** list);
void reverse_linked_list(linked_list** list);
void fatal_error(char* msg);
void append(linked_list* list, int value);

#endif