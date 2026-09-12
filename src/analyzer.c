#include <stdio.h>

#include "types.h"
#include "token.h"


static int check_str_len(char **str)
{
	if (!*str) {
		return 0;
	}
	str++;
	return *str == NULL;
}

int analyze(int size, token *tokens) 
{
	int in = 0;
	int out = 0;
	int i;

	for (i = 0; i < size; i++) {
		switch (tokens[i].type) {
			case IN:
				if (in) {
					printf("Error: double in\n");
					return 0;
				}
				in = 1;
				if (!check_str_len(tokens[++i].str)) {
					printf("Error: you can specify only one file\n");
					return 0;
				}
				break;
			case OUT:
			case APPEND:
				if (out) {
					printf("Error: double out\n");
					return 0;
				}
				out = 1;
				if (!check_str_len(tokens[++i].str)) {
					printf("Error: you can specify only one file\n");
					return 0;
				}
				break;
			case TEXT:
				in = 0;
				out = 0;
		}
	}

	return 1;
}
