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
                          5         7         9            8

                       +-----+   +-----+   +-----+      +-----+
                       |     |   |     |   |     |      |     |
Input:      *list +--->|  +----->|  +----->|  +----->...|  +-----> NULL
                       |     |   |     |   |     |      |     |
                       +-----+   +-----+   +-----+      +-----+


                          5         7         9            8

                       +-----+   +-----+   +-----+      +-----+
                       |     |   |     |   |     |      |     |
Reversed:      NULL <-----+  |<-----+  |<-----+  |...<-----+  |<---+ *list
                       |     |   |     |   |     |      |     |
                       +-----+   +-----+   +-----+      +-----+
```

Usage:

```
$ ./reverse-linked-list
5, 7, 9, 1, 2, 3, 8
8, 3, 2, 1, 9, 7, 5
```

Hash Table
----------

A [hash table] implementation using buckets and linked lists, as described in the Wikipedia article.

```
$ ./hash-table
Phone number of Lisa Smith: 521-8976.
```

Palindrome
----------

A [palindrome] is a word or sentence which reads the same backward as forward.

```
$ ./palindrome
Palindrome checker. Enter a word or sentence. CTRL-D to exit.
Input> madam
YES
Input> racecar
YES
Input> banana
NO
Input> A man, a plan, a canal, Panama!
YES
Input> Was it a car or a cat I saw?
YES
Input> No 'x' in Nixon
YES
Input> This is not a palindrome
NO
```

[reversing a singly linked list]: https://rethinkdb.com/blog/will-the-real-programmers-please-stand-up/
[hash table]: https://en.wikipedia.org/wiki/Hash_table
[palindrome]: https://en.wikipedia.org/wiki/Palindrome
[binary search tree (BST)]: https://en.wikipedia.org/wiki/Binary_search_tree
[self-balancing binary search tree]: https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree
[AVL tree]: https://en.wikipedia.org/wiki/AVL_tree
[tree traversal]: https://en.wikipedia.org/wiki/Tree_traversal