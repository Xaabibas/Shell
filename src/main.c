#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "cd.h"
#include "tokenizer.h"
#include "vector_char.h"
#include "vector_string.h"
#include "vector_token.h"

int collect_line(vector_string *args)
{	
	printf("> ");
	vector_char current_arg;
	int c = getchar();
	int is_arg = 0;
	int first_space = 0;

	if (c == EOF) {
		return 0;
	}
	vector_char_init(&current_arg);
	
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
			vector_char_push_back(&current_arg, c);
			first_space = 1;
			c = getchar();
			continue;
		}
		if (first_space) {
			vector_char_push_back(&current_arg, 0);
			vector_string_push_back(args, vector_char_get_array(&current_arg));
			vector_char_init(&current_arg);
			first_space = 0;
		}
		if (c == '\n') {
			break;
		}
		c = getchar();
	}

	if (first_space) {
		vector_char_push_back(&current_arg, 0);
		vector_string_push_back(args, vector_char_get_array(&current_arg));
		vector_char_init(&current_arg);
	}

	vector_char_free(&current_arg);
	vector_string_push_back(args, NULL);
	if (is_arg) {
		return -1;
	}
	return 1;
}

void execute(char **args)
{
	int status;
	int pid;
	
	if (0 == strcmp(args[0], "cd")) {
		change_dir(args);
		return;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		execvp(args[0], args);
		perror(args[0]);
		exit(1);
	}
	wait(&status);
}

void process() 
{
	vector_string args;
	vector_string_init(&args);
	int result;
	char **array;

	while ((result = collect_line(&args))) {
		if (result == 1) {
			array = vector_string_get_array(&args);
			if (*array) {
				execute(array);
			}
		} else if (result == -1) {
			printf("Error: Unmatched quetes\n");
		}
		vector_string_free(&args);
		vector_string_init(&args);
	}
}

void alt_process()
{
	vector_token tokens;
	vector_token_init(&tokens);


	int result;
	char **array;

	while (!(result = parse_line(&tokens))) {
		if (result == 0) {
			array = vector_token_get_array(&tokens)[0].str;
			if (*array) {
				execute(array);
			}
		}
	}
}

int main()
{
	process();

	return 0;
}
