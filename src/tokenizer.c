#include <stdio.h>

#include "vector_char.h"
#include "vector_string.h"
#include "vector_token.h"
#include "types.h"
#include "token.h"


static int is_spec(int c) 
{
	return c == '&' || c == '<' || c == '>' || c == '|' || c == ';' || c == '(' || c == ')';
}

static int is_whitespace(int c)
{
	return c == ' ' || c == '\t';
}

static void push_spec(vector_token *tokens, int c) 
{
	token t;
	t.str = NULL;
	if (c == '&' || c == '|' || c == '>') {
		int next = getchar();
		if (c == next) {
			switch (c) {
				case '&':
					t.type = AND;
					break;
				case '|':
					t.type = OR;
					break;
				case '>':
					t.type = APPEND;
			}
			return;
		} else {
			ungetc(next, stdin);
		}
	}
	switch (c) {
		case '&':
			t.type = DEAMON;
			break;
		case '>':
			t.type = OUT;
			break;
		case '<':
			t.type = IN;
			break;
		case '|':
			t.type = PIPELINE;
			break;
		case ';':
			t.type = SEQUENCE;
			break;
		case '(':
			t.type = L_BRACKET;
			break;
		case ')':
			t.type = R_BRACKET;
	}
	vector_token_push_back(tokens, t);
}

static void push_command(vector_token *tokens, vector_string *command)
{
	token t;
	vector_string_push_back(command, NULL);
	t.type = TEXT;
	t.str = vector_string_get_array(command);
	vector_token_push_back(tokens, t);

	vector_string_init(command);
}

static void push_arg(vector_string *command, vector_char *arg)
{
	vector_char_push_back(arg, 0);
	vector_string_push_back(command, vector_char_get_array(arg));
	vector_char_init(arg);
}

int parse_line(vector_token *tokens)
{
	int c = getchar();
	int arg_mode = 0;
	int invalid_syntax = 0;
	vector_string command;
	vector_char arg;

	if (c == EOF) {
		return -1;
	}
	vector_string_init(&command);
	vector_char_init(&arg);
	
	while (1) {
		while (c == '"') {
			arg_mode = ~arg_mode;
			c = getchar();
		}
		if (arg_mode || !(is_whitespace(c) || is_spec(c) || c == '\n')) {
			if (c == '\\') {
				c = getchar();
			}
			vector_char_push_back(&arg, c);
			c = getchar();
		} else if (is_whitespace(c) && vector_char_size(&arg)) {
			push_arg(&command, &arg);
			while (is_whitespace(c)) {
				c = getchar();
			}
		} else if (is_spec(c)) {
			if (vector_char_size(&arg)) {
				push_arg(&command, &arg);
			}
			if (!vector_string_size(&command)) {
				invalid_syntax = 1;
				break;
			}
			push_command(tokens, &command);
			push_spec(tokens, c);
			c = getchar();
		} else if (c == '\n') {
			break;
		} else {
			c = getchar();
		}
	}
	if (vector_char_size(&arg)) {
		push_arg(&command, &arg);
	}

	if (vector_string_size(&command)) {
		push_command(tokens, &command);
	}

	vector_char_free(&arg);
	vector_string_free(&command);
	
	if (invalid_syntax) {
		return 1;
	}
	if (arg_mode) {
		return 2;
	}

	return 0;
}
