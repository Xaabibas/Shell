#include "vector.h"
#include "tokenizer.h"


MAKE_VECTOR_CODE(vector_token, token);

void vector_token_free(vector_token *vector)
{
	int size = vector_token_size(vector);
	token *tokens = vector_token_get_array(vector);
	int i;
	char **str;

	for (i = 0; i < size; i++) {
		str = tokens[i].str;
		if (!str) {
			continue;
		}
		while (*str) {
			free(*str);
			str++;
		}
	}

	vector_token_clear(vector);
}
