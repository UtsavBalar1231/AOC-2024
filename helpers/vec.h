#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	TYPE_INT,
	TYPE_CHAR,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_VEC,
	TYPE_UNKNOWN
} vec_type_t;

typedef struct vec_t {
	size_t cap;
	size_t size;
	char *data;
	vec_type_t type;
} vec_t;

struct vec_t vec_new(vec_type_t type);
void vec_free(vec_t *v);
void *vec_at(vec_t *v, const size_t index);
size_t vec_size(vec_t *v);
size_t vec_capacity(vec_t *v);
void vec_insert(vec_t *v, const size_t index, const void *item);
void vec_push(vec_t *v, const void *item);
void *vec_pop(vec_t *v);
void vec_remove(vec_t *v, const size_t index);
void vec_print(vec_t *v);
