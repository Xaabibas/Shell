#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"
#include "types.h"
#include "tokenizer.h"
#include "vector_token.h"


void process()
{
	vector_token tokens;

	int result;

	while (1) {
		vector_token_init(&tokens);
		printf("> ");
		result = parse_line(&tokens);
		switch (result) {
			case 0:
				if (!vector_token_size(&tokens)) {
					break;
				}
				execute(vector_token_get_array(&tokens), vector_token_size(&tokens));
				break;
			case 1:
				printf("Error: Invalid syntax\n");
				break;
			case 2:
				printf("Error: Unmatched quetes\n");
				break;
		}
		if (result == -1) {
			break;
		}
		vector_token_free(&tokens);
	}
}

int main()
{
	process();

	return 0;
}
