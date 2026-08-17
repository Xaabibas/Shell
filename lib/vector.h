#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#include "codes.h"


#define DEFAULT_CAPACITY 10

#define MAKE_VECTOR_HEADER(NAME, TYPE) \
\
typedef struct NAME ## _tag { \
	TYPE *array; \
	int size; \
	int capacity; \
} NAME; \
int NAME ## _init(NAME *vector); \
int NAME ## _push_back(NAME *vector, TYPE item); \
int NAME ## _pop_back(NAME *vector, TYPE item); \
TYPE * NAME ##_get_array(NAME *vector); \
void NAME ## _clear(NAME *vector); \
int NAME ## _is_empty(NAME *vector);

#define MAKE_VECTOR_CODE(NAME, TYPE) \
typedef struct NAME ## _tag { \
	TYPE *array; \
	int size; \
	int capacity; \
} NAME; \
\
static void copy(TYPE *src, TYPE *dest, int size) \
{ \
	int i; \
	for (i = 0; i < size; i++) { \
		dest[i] = src[i]; \
	} \
} \
\
int NAME ## _init(NAME *vector) \
{ \
	vector->capacity = DEFAULT_CAPACITY; \
	vector->size = 0; \
	vector->array = malloc(vector->capacity * sizeof(TYPE)); \
	if (vector->array == NULL) { \
		return MALLOC_FAILURE; \
	} \
	return OK_CODE; \
} \
\
int NAME ## _push_back(NAME *vector, TYPE item) \
{ \
	if (vector->size >= vector->capacity) { \
		TYPE *tmp = malloc(vector->capacity * 2 * sizeof(TYPE)); \
		if (tmp == NULL) { \
			return MALLOC_FAILURE; \
		} \
		copy(vector->array, tmp, vector->size); \
		vector->capacity *= 2; \
		free(vector->array); \
		vector->array = tmp; \
	} \
	vector->array[vector->size] = item; \
	vector->size++; \
	return OK_CODE; \
} \
\
int NAME ## _pop_back(NAME *vector) \
{ \
	if (vector->size == 0) { \
		return EMPTY_ENTITY; \
	} \
	if (vector->size < vector->capacity / 2) { \
		TYPE *tmp = malloc(vector->capacity / 2 * sizeof(TYPE)); \
		if (tmp == NULL) { \
			return MALLOC_FAILURE; \
		} \
		copy(vector->array, tmp, vector->size); \
		vector->capacity /= 2; \
		free(vector->array); \
		vector->array = tmp; \
	} \
	vector->size--; \
	return OK_CODE; \
} \
\
TYPE * NAME ##_get_array(NAME *vector) \
{ \
	return vector->array; \
} \
\
int NAME ## _is_empty(NAME *vector) \
{ \
	return vector->size == 0; \
} \
\
void NAME ## _clear(NAME *vector) \
{ \
	free(vector->array); \
	vector->array = NULL; \
	vector->size = 0; \
	vector->capacity = 0; \
}

#endif
