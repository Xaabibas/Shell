#include "vector.h"

MAKE_VECTOR_CODE(vector_char, char);

void vector_char_free(vector_char *vector) 
{
	vector_char_clear(vector);
}
