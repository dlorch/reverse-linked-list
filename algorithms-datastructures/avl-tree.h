#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdbool.h>

/* misc */

void fatal_error(char* msg);
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* binary search tree */

typedef struct _binary_search_tree {
    struct _binary_search_tree* left;
    struct _binary_search_tree* right;
    int value;
} binary_search_tree;

binary_search_tree* binary_search_tree_new(int value);
void binary_search_tree_destroy(binary_search_tree** tree);
void binary_search_tree_insert(binary_search_tree** tree, int value);
int binary_search_tree_height(binary_search_tree* node);
int binary_search_tree_balance_factor(binary_search_tree* node);
void binary_search_tree_rotate_right(binary_search_tree** tree);
void binary_search_tree_rotate_left(binary_search_tree** tree);

/* queue */

typedef struct _linked_list {
    struct _linked_list* next;
    struct _linked_list* prev;
    binary_search_tree* node;
} linked_list;

typedef struct _queue {
    linked_list* first;
    linked_list* last;
    int size;
} queue;

queue* queue_new();
void queue_destroy(queue** queue);
void enqueue(queue* queue, binary_search_tree* node);
binary_search_tree* dequeue(queue* queue);
bool queue_is_empty(queue* queue);

/* traversal */

void depth_first_search_pre_order_traversal(binary_search_tree* tree);
void depth_first_search_in_order_traversal(binary_search_tree* tree);
void depth_first_search_post_order_traversal(binary_search_tree* tree);
void breadth_first_search_level_order_traversal(binary_search_tree* tree);
void visit_node(binary_search_tree* tree);

#endif