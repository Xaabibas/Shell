#ifndef VECTOR_TOKEN_H
#define VECTOR_TOKEN_H

#include "vector.h"
#include "token.h"

MAKE_VECTOR_HEADER(vector_token, token);

void vector_token_free(vector_token *vector);

#endif
