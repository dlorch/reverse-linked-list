/*
	AVL tree (self-balancing binary search tree) implementation written in C

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
#include "avl-tree.h"

/* misc */

void fatal_error(char* msg) {
	fprintf(stderr, "%s", msg);
	exit(1);
}

/* binary search tree */

binary_search_tree* binary_search_tree_new(int value) {
	binary_search_tree* tree;

	tree = (binary_search_tree*)malloc(sizeof(binary_search_tree));

	if(tree == (binary_search_tree*)NULL) {
		fatal_error("malloc failed\n");
	}

	tree->value = value;
	tree->left = (binary_search_tree*)NULL;
	tree->right = (binary_search_tree*)NULL;

	return tree;
}

void binary_search_tree_destroy(binary_search_tree** tree) {
	if((*tree)->left != (binary_search_tree*)NULL) {
		binary_search_tree_destroy(&((*tree)->left));
	}

	if((*tree)->right != (binary_search_tree*)NULL) {
		binary_search_tree_destroy(&((*tree)->right));
	}

	free(*tree);

	*tree = (binary_search_tree*)NULL;
}

void binary_search_tree_insert(binary_search_tree** tree, int value) {
	if(value > (*tree)->value) {
		if((*tree)->right == (binary_search_tree*)NULL) {
			(*tree)->right = binary_search_tree_new(value);
		} else {
			binary_search_tree_insert(&((*tree)->right), value);
		}
	} else if(value < (*tree)->value) {
		if((*tree)->left == (binary_search_tree*)NULL) {
			(*tree)->left = binary_search_tree_new(value);
		} else {
			binary_search_tree_insert(&((*tree)->left), value);
		}
	}
	
	int balance_factor = binary_search_tree_balance_factor(*tree);

	if(balance_factor > 1) {
		binary_search_tree_rotate_left(tree);
	} else if(balance_factor < -1) {
		binary_search_tree_rotate_right(tree);
	}
}

int binary_search_tree_height(binary_search_tree* node) {
	int height;

	if(node == (binary_search_tree*)NULL) {
		height = 0;
	} else {
		height = 1 + MAX(binary_search_tree_height(node->left), binary_search_tree_height(node->right));
	}

	return height;
}

int binary_search_tree_balance_factor(binary_search_tree* node) {
	int balance_factor;

	balance_factor = binary_search_tree_height(node->right) - binary_search_tree_height(node->left);

	return balance_factor;
}

void binary_search_tree_rotate_right(binary_search_tree** tree) {
	binary_search_tree* left_subtree = (*tree)->left;

	(*tree)->left = left_subtree->right;
	left_subtree->right = *tree;
	*tree = left_subtree;
}

void binary_search_tree_rotate_left(binary_search_tree** tree) {
	binary_search_tree* right_subtree = (*tree)->right;

	(*tree)->right = right_subtree->left;
	right_subtree->left = *tree;
	*tree = right_subtree;
}

/* traversal */

void depth_first_search_pre_order_traversal(binary_search_tree* tree) {
	if(tree != (binary_search_tree*)NULL) {
		visit_node(tree);
		depth_first_search_pre_order_traversal(tree->left);
		depth_first_search_pre_order_traversal(tree->right);
	}
}

void depth_first_search_in_order_traversal(binary_search_tree* tree) {
	if(tree != (binary_search_tree*)NULL) {
		depth_first_search_in_order_traversal(tree->left);
		visit_node(tree);
		depth_first_search_in_order_traversal(tree->right);
	}
}

void depth_first_search_post_order_traversal(binary_search_tree* tree) {
	if(tree != (binary_search_tree*)NULL) {
		depth_first_search_post_order_traversal(tree->left);
		depth_first_search_post_order_traversal(tree->right);
		visit_node(tree);
	}
}

void breadth_first_search_level_order_traversal(binary_search_tree* tree) {
	queue* q = queue_new();
	enqueue(q, tree);

	while(!queue_is_empty(q)) {
		binary_search_tree* node = dequeue(q);
		visit_node(node);

		if(node->left != (binary_search_tree*)NULL) {
			enqueue(q, node->left);
		}

		if(node->right != (binary_search_tree*)NULL) {
			enqueue(q, node->right);
		}
	}

	queue_destroy(&q);
}

void visit_node(binary_search_tree* tree) {
	printf("%d ", tree->value);
}

void write_dotfile(char* filename, binary_search_tree* tree) {
	FILE *fp = fopen(filename, "w");
	queue* q = queue_new();

	fprintf(fp, "digraph avl_tree {\n");

	enqueue(q, tree);

	while(!queue_is_empty(q)) {
		binary_search_tree* node = dequeue(q);

		if(node->left != (binary_search_tree*)NULL) {
			fprintf(fp, "  %d -> %d;\n", node->value, node->left->value);
			enqueue(q, node->left);
		}

		if(node->right != (binary_search_tree*)NULL) {
			fprintf(fp, "  %d -> %d;\n", node->value, node->right->value);
			enqueue(q, node->right);
		}
	}

	queue_destroy(&q);

	fprintf(fp, "}\n");
	fclose(fp);
}

/* queue */

queue* queue_new() {
	queue* q = calloc(sizeof(queue), 1);

	if(q == (queue*)NULL) {
		fatal_error("calloc failed");
	}

	return q;
}

void queue_destroy(queue** q) {
	linked_list* current = (*q)->first;
	linked_list* next;

	while(current != (linked_list*)NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	free(*q);

	*q = (queue*)NULL;
}

void enqueue(queue* queue, binary_search_tree* node) {
	linked_list* list = malloc(sizeof(linked_list));

	if(list == (linked_list*)NULL) {
		fatal_error("malloc failed");
	}

	list->node = node;

	if(queue->size == 0) {
		list->next = (linked_list*)NULL;
		list->prev = (linked_list*)NULL;
		queue->first = list;
		queue->last = list;
		queue->size = 1;
	} else {
		list->next = queue->first;
		list->prev = (linked_list*)NULL;
		queue->first->prev = list;
		queue->first = list;
		queue->size += 1;
	}
}

binary_search_tree* dequeue(queue* queue) {
	binary_search_tree* result = (binary_search_tree*)NULL;

	if(queue->size > 0) {
		result = queue->last->node;

		if(queue->size == 1) {
			free(queue->first);
			queue->first = (linked_list*)NULL;
			queue->last = (linked_list*)NULL;
			queue->size = 0;
		} else {
			linked_list* prev = queue->last->prev;
			free(queue->last);
			prev->next = (linked_list*)NULL;
			queue->last = prev;
			queue->size -= 1;
		}
	}

	return result;
}

bool queue_is_empty(queue* queue) {
	return queue->size == 0;
}

// AVL tree: https://en.wikipedia.org/wiki/AVL_tree
int main() {
	binary_search_tree* tree = binary_search_tree_new(9);
	char* filename = "avl-tree.dot";

	binary_search_tree_insert(&tree, 12);
	binary_search_tree_insert(&tree, 14);
	binary_search_tree_insert(&tree, 17);
	binary_search_tree_insert(&tree, 19);
	binary_search_tree_insert(&tree, 23);
	binary_search_tree_insert(&tree, 54);
	binary_search_tree_insert(&tree, 67);
	binary_search_tree_insert(&tree, 72);
	binary_search_tree_insert(&tree, 76);

	printf("Height: %d\n", binary_search_tree_height(tree));
	printf("Left sub-tree height: %d\n", binary_search_tree_height(tree->left));
	printf("Right sub-tree height: %d\n", binary_search_tree_height(tree->right));
	printf("Balance factor: %d\n", binary_search_tree_balance_factor(tree));

	printf("Depth first search, pre-order traversal: ");
	depth_first_search_pre_order_traversal(tree);
	printf("\n");

	printf("Depth first search, in-order traversal: ");
	depth_first_search_in_order_traversal(tree);
	printf("\n");

	printf("Depth first search, post-order traversal: ");
	depth_first_search_post_order_traversal(tree);
	printf("\n");

	printf("Breadth first search, level-order traversal: ");
	breadth_first_search_level_order_traversal(tree);
	printf("\n");

	write_dotfile(filename, tree);
	printf("Wrote \"%s\"\n", filename);

	binary_search_tree_destroy(&tree);

    return 0;
}