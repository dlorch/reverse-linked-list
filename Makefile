all: reverse-linked-list hash-table
.PHONY: all

reverse-linked-list: reverse-linked-list.c reverse-linked-list.h
	gcc -std=c99 -Wall -o reverse-linked-list reverse-linked-list.c

hash-table: hash-table.c hash-table.h
	gcc -std=c99 -Wall -o hash-table hash-table.c