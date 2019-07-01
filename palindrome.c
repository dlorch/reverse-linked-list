/*
	Palindrome checker

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
#include <stdlib.h>
#include "palindrome.h"

// "A man, a plan, a canal, Panama!", "Was it a car or a cat I saw?" or "No 'x' in Nixon"

char* chars_only(char* str) {
    char* result = strdup(str); // result has same or smaller size than input string
    int str_index = 0;
    int result_index = 0;

    for(str_index = 0; str_index < strlen(str); str_index++) {
        if(isalpha(str[str_index])) {
            result[result_index] = tolower(str[str_index]);
            result_index++;
        }
    }

    result[result_index] = '\0';

    return result;
}

bool is_palindrome(char* str) {
    int length = strlen(str);
    bool result = true;

    if(length == 0) {
        result = false;
    } else {
        int middle = length / 2;
        for(int i=0; i<middle; i++) {
            if(str[i] != str[length - 1 - i]) {
                result = false;
                break;
            }
        }
    }

    return result;
}

void trim_newline(char* str) {
    int len = strlen(str);
    if(str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    char sentence[LINE_MAX];
    bool loop = true;

    printf("Palindrome checker. Enter a word or sentence. CTRL-D to exit.\n");

    while(loop) {
        printf("Input> ");

        if(fgets(sentence, LINE_MAX, stdin) == NULL) { // length count in fgets() includes the final '\0' character
            loop = false;
        } else {
            trim_newline(sentence);

            char* chars = chars_only(sentence);

            if(strlen(chars) > 0) {
                if(is_palindrome(chars)) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
            }

            free(chars);
        }
    }

    return 0;
}
