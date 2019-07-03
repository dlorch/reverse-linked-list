#ifndef PARENTHESIS_H
#define PARENTHESIS_H

#include <stdbool.h>

/* misc */

void fatal_error(char* msg);
void trim_newline(char* str);

/* parenthesis */

bool parenthesis_match(char* str);

/* stack */

typedef struct _linked_list {
    struct _linked_list* next;
    struct _linked_list* prev;
    char parenthesis;
} linked_list;

typedef struct _stack {
    linked_list* first;
    linked_list* last;
    int size;
} stack;

stack* stack_new();
void stack_destroy(stack** s);
void push(stack* stack, char parenthesis);
char pop(stack* stack);

#endif