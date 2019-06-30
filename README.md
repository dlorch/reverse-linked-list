Reversing a Singly Linked List and Other Brain Teasers
======================================================

This repository contains random code on interesting challenges. The repository was named
after the mother of all challenges: [reversing a singly linked list].

Build all programs using:

```
$ make
```

Reversing a Singly Linked List
------------------------------

Reversing a singly linked list *in-place*: traverse the linked list element by element
and "flip" the next-pointers "backwards".

```
                         5       7       9          8

                       +---+   +---+   +---+      +---+
                       |   |   |   |   |   |      |   |
Input:      list* +--->+ +---->+ +---->+ +---->...| +----> NULL
                       |   |   |   |   |   |      |   |
                       +---+   +---+   +---+      +---+


                         5       7       9          8

                       +---+   +---+   +---+      +---+
                       |   |   |   |   |   |      |   |
Reversed:      NULL <----+ +<----+ +<----+ |...<----+ +<----+ list*
                       |   |   |   |   |   |      |   |
                       +---+   +---+   +---+      +---+
```

Usage:

```
$ ./reverse-linked-list
5, 7, 9, 1, 2, 3, 8
8, 3, 2, 1, 9, 7, 5
```

Hash Table
----------

[Hash table] implementation using buckets and linked lists, as described in the Wikipedia article.

```
$ ./hash-table
Phone number of Lisa Smith: 521-8976.
```

[reversing a singly linked list]: https://rethinkdb.com/blog/will-the-real-programmers-please-stand-up/
[Hash table]: https://en.wikipedia.org/wiki/Hash_table