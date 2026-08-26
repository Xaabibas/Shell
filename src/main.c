#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "vector_char.h"
#include "vector_string.h"

void process() 
{
	vector_char vector;
	vector_string args;
	char *command;
	int pid;
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
		vector_string_init(&args);
		
		is_arg = 0;
		first_space = 0;
		
		while (1) {
			while (c == '"') {
				is_arg = ~is_arg;
				c = getchar();
				first_space = 1;
			}
			if (is_arg || (c != ' ' && c != '\t' && c != '\n')) {
				if (c == '\\') {
					c = getchar();
				}
				vector_char_push_back(&vector, c);
				first_space = 1;
				c = getchar();
				continue;
			}
			if (first_space) {
				vector_char_push_back(&vector, 0);
				vector_string_push_back(&args, vector_char_get_array(&vector));
				vector_char_init(&vector);
				first_space = 0;
			}
			if (c == '\n') {
				break;
			}
			c = getchar();
		}
		
		if (first_space) {
			vector_char_push_back(&vector, 0);
			vector_string_push_back(&args, vector_char_get_array(&vector));
			vector_char_init(&vector);
		}

		if (is_arg) {
			printf("Error: unmatched quotes\n");
			while (!vector_string_is_empty(&args)) {
				/* Free arrays */
			}
			vector_char_clear(&vector);
			continue;
		}
			
		command = vector_string_get_array(&args)[0];

		pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(1);	
		}

		if (pid == 0) {
			execvp(command, vector_string_get_array(&args));
			perror(command);
			exit(1);
		}
		wait(NULL);

		vector_string_clear(&args);
		vector_char_clear(&vector);
	}

	
}

int main()
{
	process();

	return 0;
}
