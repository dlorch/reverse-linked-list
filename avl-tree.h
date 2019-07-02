#ifndef AVL_TREE_H
#define AVL_TREE_H

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

/* traversal */

void depth_first_search_pre_order_traversal(binary_search_tree* tree);
void depth_first_search_in_order_traversal(binary_search_tree* tree);
void depth_first_search_post_order_traversal(binary_search_tree* tree);
void breadth_first_search_level_order_traversal(binary_search_tree* tree);
void visit_node(binary_search_tree* tree);

#endif