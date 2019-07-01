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
#include "palindrome.h"

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

int main() {
    char input[LINE_MAX];
    bool loop = true;

    printf("Palindrome checker. Enter empty line to exit.\n");

    while(loop) {
        printf("Enter word> ");
        fgets(input, LINE_MAX, stdin); // length count includes the final '\0' character

        // remove newline
        int i = strlen(input)-1;
        if(input[i] == '\n') 
            input[i] = '\0';

        if(strlen(input) == 0) {
            loop = false;
        } else {
            if(is_palindrome(input)) {
                printf("'%s' is a palindrome!\n", input);
            } else {
                printf("'%s' is not a palindrome :-(\n", input);
            }
        }
    }

    return 0;
}