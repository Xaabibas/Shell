#ifndef VECTOR_STRING_H
#define VECTOR_STRING_H

#include "vector.h"

MAKE_VECTOR_HEADER(vector_string, char *);

void vector_string_free(vector_string *vector);

#endif
