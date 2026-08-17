#include <stdio.h>
#include <stdlib.h>

#include "vector_char.h"
#include "list_string.h"


void process() 
{
	list_string list;
	list_string_init(&list); 
	vector_char vector;
	int is_arg;
	int first_space;
	int c;

	while (1) {
		printf("> ");
		c = getchar();
		if (c == EOF) {
			break;
		}
		vector_char_init(&vector);
		is_arg = 0;
		first_space = 0;
		
		while (c != '\n') {
			if (c == '"') {
				is_arg = ~is_arg;
				c = getchar();
				continue;
			}
			if (is_arg || (c != ' ' && c != '\t')) {
				vector_char_push_back(&vector, c);
				first_space = 1;
				c = getchar();
				continue;
			}
			if (first_space) {
				vector_char_push_back(&vector, 0);
				list_string_push(&list, vector_char_get_array(&vector));
				vector_char_init(&vector);
				first_space = 0;
			}
			c = getchar();
		}
		
		if (first_space) {
			vector_char_push_back(&vector, 0);
			list_string_push(&list, vector_char_get_array(&vector));
			vector_char_init(&vector);
		}

		if (is_arg) {
			printf("Error: unmatched quotes\n");
			while (!list_string_is_empty(&list)) {
				char *string = list_string_front(&list);
				list_string_pop(&list);
				free(string);
			}
			vector_char_clear(&vector);
			continue;
		}

		while (!list_string_is_empty(&list)) {
			char *string = list_string_front(&list);

			printf("[%s]\n", string);

			list_string_pop(&list);
			free(string);
		}

		vector_char_clear(&vector);
	}

	
}

int main()
{
	process();

	return 0;
}
