/*
	Matching parenthesis checker

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
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "parenthesis.h"

/* misc */

void fatal_error(char* msg) {
    fprintf(stderr, "%s", msg);
    exit(1);
}

void trim_newline(char* str) {
    int len = strlen(str);
    if(str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/* parenthesis */

bool parenthesis_match(char* str) {
    bool result = true;
    stack* s = stack_new();
    char c;

    for(int i=0; i<strlen(str) && result; ++i) {
        switch(str[i]) {
            case '(':
                push(s, '(');
                break;
            case ')':
                c = pop(s);
                result = (c == '(');
                break;
            case '[':
                push(s, '[');
                break;
            case ']':
                c = pop(s);
                result = (c == '[');
                break;
            case '{':
                push(s, '{');
                break;
            case '}':
                c = pop(s);
                result = (c == '{');
                break;
        }
    }

    if(s->size > 0) {
        result = false;
    }

    stack_destroy(&s);

    return result;
}

/* stack */

stack* stack_new() {
	stack* s = calloc(sizeof(stack), 1);

	if(s == (stack*)NULL) {
		fatal_error("calloc failed");
	}

	return s;
}

void stack_destroy(stack** s) {
	linked_list* current = (*s)->first;
    linked_list* next;

	while(current != (linked_list*)NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	free(*s);

	*s = (stack*)NULL;
}

void push(stack* stack, char parenthesis) {
	linked_list* list = malloc(sizeof(linked_list));

	if(list == (linked_list*)NULL) {
		fatal_error("malloc failed");
	}

	list->parenthesis = parenthesis;

	if(stack->size == 0) {
		list->next = (linked_list*)NULL;
		list->prev = (linked_list*)NULL;
		stack->first = list;
		stack->last = list;
		stack->size = 1;
	} else {
		list->next = (linked_list*)NULL;
		list->prev = stack->last;
		stack->last->next = list;
		stack->last = list;
		stack->size += 1;
	}
}

char pop(stack* stack) {
	char result = 0;

	if(stack->size > 0) {
		result = stack->last->parenthesis;

		if(stack->size == 1) {
			free(stack->first);
			stack->first = (linked_list*)NULL;
			stack->last = (linked_list*)NULL;
			stack->size = 0;
		} else {
			linked_list* prev = stack->last->prev;
			free(stack->last);
			prev->next = (linked_list*)NULL;
			stack->last = prev;
			stack->size -= 1;
		}
	}

	return result;
}

int main() {
    char sentence[LINE_MAX];
    bool loop = true;

    printf("Matching parenthesis checker. Supports {}, () and []. CTRL-D to exit.\n");

    while(loop) {
        printf("Input> ");

        if(fgets(sentence, LINE_MAX, stdin) == NULL) { // length count in fgets() includes the final '\0' character
            loop = false;
        } else {
            if(strlen(sentence) > 0) {
                if(parenthesis_match(sentence)) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
            }
        }
    }

    return 0;
}