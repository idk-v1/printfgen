#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

#define List(type, name) \
typedef struct name \
{ \
	uint64_t size; \
	uint64_t _max; \
	type* data; \
} name; \
static inline name name##_create() \
{ \
	name list = { .size = 0, ._max = 32, .data = malloc(sizeof(type) * list._max) }; \
	if (!list.data) \
		list._max = 0; \
	return list; \
} \
static inline void name##_free(name* list) \
{ \
	if (list->data) \
		free(list->data); \
	list->size = 0; \
	list->_max = 32; \
} \
static inline void name##_push(name* list, type add) \
{ \
	if (list->size >= list->_max) \
	{ \
		list->_max *= 2; \
		void* ptr = realloc(list->data, sizeof(type) * list->_max); \
		if (!ptr) \
			name##_free(list); \
		else \
		{ \
			list->data = (type*)ptr; \
			list->data[list->size++] = add; \
		} \
	} \
	else \
		list->data[list->size++] = add; \
} \
static inline void name##_pop(name* list) \
{ \
	if (list->size > 0) \
		list->size--; \
}