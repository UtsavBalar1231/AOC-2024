#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/// Helper function to determine the size of a type
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

/// Allocate and initialize a new vector
vec_t *vec_create(vec_type_t type)
{
	vec_t *v = malloc(sizeof(vec_t));
	if (!v) {
		fprintf(stderr,
			"ERROR: Failed to allocate memory for vector\n");
		exit(EXIT_FAILURE);
	}

	v->data =
		malloc(vec_type_size(type) * 8); // Default initial capacity: 8
	if (!v->data) {
		free(v);
		fprintf(stderr, "ERROR: Failed to allocate vector data\n");
		exit(EXIT_FAILURE);
	}

	v->size = 0;
	v->cap = 8;
	v->type = type;
	return v;
}

/// Free memory associated with a vector
void vec_destroy(vec_t *v)
{
	if (!v)
		return;

	if (v->type == TYPE_VEC) {
		for (size_t i = 0; i < v->size; i++) {
			vec_t *nested_vec = (vec_t *)vec_at(v, i);
			vec_destroy(nested_vec);
		}
	}

	free(v->data);
	free(v);
}

/// Get the size (number of elements) of the vector
size_t vec_size(const vec_t *v)
{
	return v ? v->size : 0;
}

/// Get the capacity of the vector
size_t vec_capacity(const vec_t *v)
{
	return v ? v->cap : 0;
}

/// Access an element by index
void *vec_at(const vec_t *v, size_t index)
{
	assert(v && index < v->size && "Index out of bounds");
	return (char *)v->data + index * vec_type_size(v->type);
}

/// Resize vector if necessary to accommodate more elements
static void vec_resize_if_needed(vec_t *v)
{
	if (v->size >= v->cap) {
		size_t new_cap = v->cap * 2;
		void *new_data =
			realloc(v->data, new_cap * vec_type_size(v->type));
		if (!new_data) {
			fprintf(stderr, "ERROR: Failed to resize vector\n");
			exit(EXIT_FAILURE);
		}
		v->data = new_data;
		v->cap = new_cap;
	}
}

/// Add an element to the end of the vector
void vec_push_back(vec_t *v, const void *item)
{
	assert(v && item);
	vec_resize_if_needed(v);
	memcpy((char *)v->data + v->size * vec_type_size(v->type), item,
	       vec_type_size(v->type));
	v->size++;
}

/// Remove and return the last element of the vector
void *vec_pop_back(vec_t *v)
{
	assert(v && v->size > 0 && "Vector is empty");
	v->size--;
	return (char *)v->data + v->size * vec_type_size(v->type);
}

/// Insert an element at a specific index
void vec_insert(vec_t *v, size_t index, const void *item)
{
	assert(v && item && index <= v->size && "Index out of bounds");
	vec_resize_if_needed(v);

	// Shift elements to the right
	memmove((char *)v->data + (index + 1) * vec_type_size(v->type),
		(char *)v->data + index * vec_type_size(v->type),
		(v->size - index) * vec_type_size(v->type));

	// Insert the item
	memcpy((char *)v->data + index * vec_type_size(v->type), item,
	       vec_type_size(v->type));
	v->size++;
}

/// Remove an element at a specific index
void vec_erase(vec_t *v, size_t index)
{
	assert(v && index < v->size && "Index out of bounds");

	// Shift elements to the left
	memmove((char *)v->data + index * vec_type_size(v->type),
		(char *)v->data + (index + 1) * vec_type_size(v->type),
		(v->size - index - 1) * vec_type_size(v->type));

	v->size--;
}

/// Print the contents of the vector
void vec_print(const vec_t *v)
{
	if (!v || !v->data) {
		printf("[]\n");
		return;
	}

	printf("[");
	for (size_t i = 0; i < v->size; i++) {
		void *item = vec_at(v, i);
		switch (v->type) {
		case TYPE_INT:
			printf("%d", *(int *)item);
			break;
		case TYPE_FLOAT:
			printf("%f", *(float *)item);
			break;
		case TYPE_CHAR:
			printf("'%c'", *(char *)item);
			break;
		case TYPE_STRING:
			printf("\"%s\"", *(char **)item);
			break;
		case TYPE_VEC:
			vec_print((vec_t *)item);
			break;
		default:
			fprintf(stderr, "ERROR: Unknown type\n");
			exit(EXIT_FAILURE);
		}

		if (i < v->size - 1)
			printf(", ");
	}
	printf("]\n");
}

vec_t *vec_copy(vec_t *v) {
    if (!v) {
        return NULL;
    }

    vec_t *copy = vec_create(v->type);
    copy->size = v->size;
    copy->cap = v->cap;
    copy->data = malloc(vec_type_size(v->type) * v->cap);
    assert(copy->data != NULL && "Failed to allocate memory for vector copy");

    if (v->type == TYPE_VEC) {
        // Deep copy nested vectors
        for (size_t i = 0; i < v->size; i++) {
            vec_t *nested_vec = (vec_t *)vec_at(v, i);
            vec_t *nested_copy = vec_copy(nested_vec);
            vec_push_back(copy, &nested_copy);
        }
    } else if (v->type == TYPE_STRING) {
        // Deep copy strings
        for (size_t i = 0; i < v->size; i++) {
            char *original = *(char **)vec_at(v, i);
            char *copy_str = strdup(original);
            vec_push_back(copy, &copy_str);
        }
    } else {
        // Shallow copy for basic types
        memcpy(copy->data, v->data, vec_type_size(v->type) * v->size);
    }

    return copy;
}
