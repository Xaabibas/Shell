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
	return c == ' ' || c == '\t' || c == '\n';
}

static void form_token(token *t, int c) 
{
	if (c == '&' || c == '|' || c == '>') {
		int next = getchar();
		if (c == next) {
			switch (c) {
				case '&':
					t->type = AND;
					break;
				case '|':
					t->type = OR;
					break;
				case '>':
					t->type = APPEND;
			}
			return;
		} else {
			ungetc(next, stdin);
		}
	}
	switch (c) {
		case '&':
			t->type = DEAMON;
			break;
		case '>':
			t->type = OUT;
			break;
		case '<':
			t->type = IN;
			break;
		case '|':
			t->type = PIPELINE;
			break;
		case ';':
			t->type = SEQUENCE;
			break;
		case '(':
			t->type = L_BRACKET;
			break;
		case ')':
			t->type = R_BRACKET;
	}
}

int parse_line(vector_token *tokens)
{
	int c = getchar();
	int arg_mode = 0;

	vector_string command;
	vector_char arg;

	token current_token;

	if (c == EOF) {
		return -1;
	}
	vector_string_init(&command);
	vector_char_init(&arg);
	
	while (c != '\n') {
		while (c == '"') {
			arg_mode = ~arg_mode;
			c = getchar();
		}
		if (arg_mode || !(is_whitespace(c) || is_spec(c))) {
			if (c == '\\') {
				c = getchar();
			}
			vector_char_push_back(&arg, c);
			c = getchar();
		} else if (is_whitespace(c) && vector_char_size(&arg)) {
			vector_char_push_back(&arg, 0);
			vector_string_push_back(&command, vector_char_get_array(&arg));
			vector_char_init(&arg);
			while (is_whitespace(c)) {
				c = getchar();
			}
		} else if (is_spec(c)) {
			if (!vector_string_size(&command)) {
				return 1;
			}
			current_token.type = TEXT;
			current_token.str = vector_string_get_array(&command);
			vector_token_push_back(tokens, current_token);
			form_token(&current_token, c);
			vector_token_push_back(tokens, current_token);

			vector_string_init(&command);

			c = getchar();
		}
	}

	return 0;
}
