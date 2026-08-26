#include "vector.h"

MAKE_VECTOR_CODE(vector_string, char *);

void vector_string_free(vector_string *vector)
{
	int size = vector_string_size(vector);
	int i;
	char **array = vector_string_get_array(vector);
	char *string;

	for (i = 0; i < size; i++) {
		string = array[i];
		free(string);
	}

	vector_string_clear(vector);
}
