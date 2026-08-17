#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

#define MAKE_LIST_HEADER(NAME, TYPE) \
\
typedef struct NAME ## _item_tag { \
	TYPE item; \
	struct NAME ## item_tag *next; \
} NAME ## _item; \
\
typedef struct NAME ## _tag { \
	NAME ## _item *first; \
	NAME ## _item *last; \
} NAME; \
void NAME ## _init(NAME *list); \
void NAME ## _push(NAME *list, TYPE a); \
TYPE NAME ## _front(NAME *list); \
void NAME ## _pop(NAME *list); \
int NAME ## _is_empty(NAME *list); \
void NAME ## _free(NAME *list);

#define MAKE_LIST_CODE(NAME, TYPE) \
\
typedef struct NAME ## _item_tag { \
	TYPE item; \
	struct NAME ## _item_tag *next; \
} NAME ## _item; \
\
typedef struct NAME ## _tag { \
	NAME ## _item *first; \
	NAME ## _item *last; \
} NAME; \
\
void NAME ## _init(NAME *list) \
{ \
	list->first = NULL; \
	list->last = NULL; \
} \
\
void NAME ## _push(NAME *list, TYPE a) \
{ \
	NAME ## _item *new = malloc(sizeof(NAME ## _item)); \
	if (new == NULL) { \
		return; \
	} \
	new->item = a; \
	new->next = NULL; \
	if (list->last == NULL) { \
		list->first = new; \
	} else { \
		list->last->next = new; \
	} \
	list->last = new; \
} \
\
TYPE NAME ## _front(NAME *list) \
{ \
	return list->first->item; \
} \
\
void NAME ## _pop(NAME *list) \
{ \
	if (list->first == NULL) { \
		return; \
	} \
	NAME ## _item *tmp = list->first; \
	list->first = list->first->next; \
	if (list->first == NULL) { \
		list->last = NULL; \
	} \
	free(tmp); \
} \
\
int NAME ## _is_empty(NAME *list) \
{ \
	return list->first == NULL; \
} \
\
static void do_free(NAME ## _item *item) { \
	if (item == NULL) { \
		return; \
	} \
	do_free(item->next); \
	free(item); \
} \
\
void NAME ## _free(NAME *list) \
{ \
	do_free(list->first); \
	free(list); \
}

#endif
