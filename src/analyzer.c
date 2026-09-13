#include <stdio.h>

#include "types.h"
#include "token.h"


static int exactly_one_argument(char **str)
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
					printf("Error: multiple input redirection\n");
					return 0;
				}
				in = 1;
				if (i + 1 >= size || !exactly_one_argument(tokens[++i].str)) {
					printf("Error: one input file expected\n");
					return 0;
				}
				break;
			case OUT:
			case APPEND:
				if (out) {
					printf("Error: multiple output redirection\n");
					return 0;
				}
				out = 1;
				if (i + 1 >= size || !exactly_one_argument(tokens[++i].str)) {
					printf("Error: one output file expected\n");
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
