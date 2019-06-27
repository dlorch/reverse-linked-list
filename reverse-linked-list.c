/*
	Reversing a linked list in O(n) written in C

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
#include "reverse-linked-list.h"

void print_linked_list(linked_list* list) {
    linked_list* current = list;

    while(current != NULL) {
        if(current->next != NULL) {
            printf("%d, ", current->value);
        } else {
            printf("%d", current->value);
        }
        current = current->next;
    }

    printf("\n");
}

linked_list* make_linked_list(int value) {
    linked_list* list = malloc(sizeof(linked_list));
    list->next = (linked_list*)NULL;
    list->value = value;

    return list;
}

void append(linked_list* list, int value) {
    linked_list* last = list;

    // fast-forward to last element
    while(last->next != (linked_list*)NULL) {
        last = last->next;
    }

    last->next = malloc(sizeof(linked_list));
    last->next->next = NULL;
    last->next->value = value;
}

void destroy_linked_list(linked_list** list) {
    linked_list* current = *list;
    linked_list* next;

    while(current != (linked_list*)NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *list = (linked_list*)NULL;
}

linked_list* prepend(linked_list* list, int value) {
    linked_list* prepend = make_linked_list(value);
    prepend->next = list;
    return prepend;
}

// reverse a linked list in O(n)
linked_list* reverse_linked_list(linked_list* list) {
    linked_list* reverse = NULL;
    linked_list* current = list;

    while(current != NULL) {
        reverse = prepend(reverse, current->value);
        current = current->next;
    }

    return reverse;
}

int main() {
    linked_list* list = make_linked_list(5);
    append(list, 7);
    append(list, 9);
    append(list, 1);
    append(list, 2);
    append(list, 3);
    append(list, 8);
    print_linked_list(list);

    linked_list* reverse = reverse_linked_list(list);
    print_linked_list(reverse);

    destroy_linked_list(&list);
    destroy_linked_list(&reverse);

    return 0;
}