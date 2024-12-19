#ifndef VEC_H
#define VEC_H

#include <stddef.h> // For size_t

/// Enum to represent the type of elements stored in the vector
typedef enum {
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_CHAR,
	TYPE_STRING,
	TYPE_VEC
} vec_type_t;

/// Structure representing a generic dynamic vector
typedef struct {
	void *data; // Pointer to the array of elements
	size_t size; // Current number of elements
	size_t cap; // Capacity of the vector
	vec_type_t type; // Type of elements in the vector
} vec_t;

/// Function declarations

/**
 * Creates and initializes a new vector.
 *
 * @param type The type of elements to store in the vector.
 * @return Pointer to the newly created vector.
 */
vec_t *vec_create(vec_type_t type);

/**
 * Frees the memory associated with a vector.
 *
 * @param v Pointer to the vector to destroy.
 */
void vec_destroy(vec_t *v);

/**
 * Returns the current number of elements in the vector.
 *
 * @param v Pointer to the vector.
 * @return Number of elements in the vector.
 */
size_t vec_size(const vec_t *v);

/**
 * Returns the current capacity of the vector.
 *
 * @param v Pointer to the vector.
 * @return Capacity of the vector.
 */
size_t vec_capacity(const vec_t *v);

/**
 * Accesses an element at a given index.
 *
 * @param v Pointer to the vector.
 * @param index Index of the element to access.
 * @return Pointer to the element at the given index.
 */
void *vec_at(const vec_t *v, size_t index);

/**
 * Adds an element to the end of the vector.
 *
 * @param v Pointer to the vector.
 * @param item Pointer to the element to add.
 */
void vec_push_back(vec_t *v, const void *item);

/**
 * Removes and returns the last element of the vector.
 *
 * @param v Pointer to the vector.
 * @return Pointer to the removed element.
 */
void *vec_pop_back(vec_t *v);

/**
 * Inserts an element at the specified index.
 *
 * @param v Pointer to the vector.
 * @param index Index where the element should be inserted.
 * @param item Pointer to the element to insert.
 */
void vec_insert(vec_t *v, size_t index, const void *item);

/**
 * Removes an element at the specified index.
 *
 * @param v Pointer to the vector.
 * @param index Index of the element to remove.
 */
void vec_erase(vec_t *v, size_t index);

/**
 * Prints the contents of the vector.
 *
 * @param v Pointer to the vector to print.
 */
void vec_print(const vec_t *v);

/**
 * Returns a pointer to a copy of the vector
 *
 * @param v Pointer to the vector to copy
 * @return Pointer to the copied vector
 */
vec_t *vec_copy(vec_t *v);

#endif // VEC_H
