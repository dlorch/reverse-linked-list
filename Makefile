all: reverse-linked-list hash-table palindrome avl-tree
.PHONY: all

reverse-linked-list: reverse-linked-list.c reverse-linked-list.h
	gcc -std=c99 -Wall -o reverse-linked-list reverse-linked-list.c

hash-table: hash-table.c hash-table.h
	gcc -std=c99 -Wall -o hash-table hash-table.c

palindrome: palindrome.c palindrome.h
	gcc -std=c99 -Wall -o palindrome palindrome.c

avl-tree: avl-tree.c avl-tree.h
	gcc -std=c99 -Wall -o avl-tree avl-tree.c
