#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t vec_type_size(vec_type_t type)
{
	switch (type) {
	case TYPE_INT:
		return sizeof(int);
	case TYPE_FLOAT:
		return sizeof(float);
	case TYPE_CHAR:
		return sizeof(char);
	case TYPE_STRING:
		return sizeof(char *);
	case TYPE_VEC:
		return sizeof(vec_t);
	default:
		fprintf(stderr, "ERROR: Unknown vector type\n");
		exit(EXIT_FAILURE);
	}
}

struct vec_t vec_new(vec_type_t type)
{
	vec_t v = { .data = NULL, .size = 0, .cap = 8, .type = type };
	v.data = malloc(vec_type_size(type) * v.cap);
	assert(v.data != NULL && "ERROR: Failed to allocate memory for vector");
	return v;
}

void vec_free(vec_t *v)
{
	if (!v || !v->data)
		return;

	if (v->type == TYPE_VEC) {
		for (size_t i = 0; i < v->size; i++) {
			vec_t *nested_vec = (vec_t *)vec_at(v, i);
			vec_free(nested_vec);
		}
	}

	free(v->data);
	v->data = NULL;
	v->size = 0;
	v->cap = 0;
}

size_t vec_size(vec_t *v)
{
	return v ? v->size : 0;
}

size_t vec_capacity(vec_t *v)
{
	return v ? v->cap : 0;
}

void *vec_at(vec_t *v, const size_t index)
{
	assert(index >= 0 && "Index cannot be negative");
	assert(index < v->size && "Index out of bounds");
	assert(v->data != NULL && "Data is NULL");

	return (char *)v->data + index * vec_type_size(v->type);
}

static int vec_check_allocation(vec_t *v)
{
	if (v->size >= v->cap) {
		size_t new_cap = (v->cap == 0) ? 8 : v->cap * 2;
		void *new_data =
			realloc(v->data, new_cap * vec_type_size(v->type));
		v->data = new_data;
		assert(v->data != NULL &&
		       "Failed to allocate memory for vector");
		v->cap = new_cap;
	}

	return 1;
}

void vec_push(vec_t *v, const void *item)
{
	assert(v->data != NULL && "Data is NULL");
	assert(vec_check_allocation(v) &&
	       "Failed to allocate memory for vector");

	memcpy((char *)v->data + v->size * vec_type_size(v->type), item,
	       vec_type_size(v->type));
	v->size++;
}

void *vec_pop(vec_t *v)
{
	assert(v->data != NULL && "Data is NULL");
	assert(v->size > 0 && "Vector is empty");

	v->size--;
	return (char *)v->data + v->size * vec_type_size(v->type);
}

void vec_insert(vec_t *v, size_t index, const void *item)
{
	assert(index <= v->size && "Index out of bounds");
	assert(index >= 0 && "Index cannot be negative");

	assert(vec_check_allocation(v) &&
	       "Failed to allocate memory for vector");

	// Shift elements to the right
	// Example: vec:[1, 2, 3] -> insert(1, 4) -> vec:[1, 4, 2, 3]
	memmove((char *)v->data + (index + 1) * vec_type_size(v->type),
		(char *)v->data + index * vec_type_size(v->type),
		(v->size - index) * vec_type_size(v->type));

	// Insert item at index
	memcpy((char *)v->data + index * vec_type_size(v->type), item,
	       vec_type_size(v->type));
	v->size++;
}

void vec_remove(vec_t *v, const size_t index)
{
	assert(v->data != NULL && "Data is NULL");
	assert(v->size > 0 && "Vector is empty");
	assert(index <= v->size && "Index out of bounds");
	assert(index >= 0 && "Index cannot be negative");

	// Shift elements to the left
	// Example: vec:[1, 2, 3] -> remove(1) -> vec:[1, 3]
	memmove((char *)v->data + index * vec_type_size(v->type),
		(char *)v->data + (index + 1) * vec_type_size(v->type),
		(v->size - index - 1) * vec_type_size(v->type));
	v->size--;
}

// Print a vector
void vec_print(vec_t *v)
{
	if (!v || !v->data) {
		printf("[]");
		return;
	}

	printf("[");
	for (size_t i = 0; i < v->size; i++) {
		void *item = vec_at(v, i);

		switch(v->type) {
		case TYPE_INT:
			item = (int *)item;
			break;
		case TYPE_FLOAT:
			item = (float *)item;
			break;
		case TYPE_CHAR:
			item = (char *)item;
			break;
		case TYPE_STRING:
			item = (char **)item;
			break;
		case TYPE_VEC:
			item = (vec_t *)item;
			break;
		default:
			fprintf(stderr, "ERROR: Unknown vector type\n");
			exit(EXIT_FAILURE);
		}

		if (i < v->size - 1)
			printf(", ");
	}
	printf("]");
}
