#include "vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_create_destroy(void)
{
	vec_t *v = vec_create(TYPE_INT);
	assert(v != NULL);
	assert(vec_size(v) == 0);
	assert(vec_capacity(v) > 0);
	vec_destroy(v);
	printf("test_create_destroy passed.\n");
}

void test_push_pop_int(void)
{
	vec_t *v = vec_create(TYPE_INT);
	int nums[] = { 10, 20, 30 };

	for (size_t i = 0; i < 3; i++) {
		vec_push_back(v, &nums[i]);
		assert(*(int *)vec_at(v, i) == nums[i]);
	}

	assert(vec_size(v) == 3);

	for (int i = 2; i >= 0; i--) {
		int *popped = (int *)vec_pop_back(v);
		assert(*popped == nums[i]);
	}

	assert(vec_size(v) == 0);
	vec_destroy(v);
	printf("test_push_pop_int passed.\n");
}

void test_push_pop_string(void)
{
	vec_t *v = vec_create(TYPE_STRING);
	const char *strings[] = { "apple", "banana", "cherry" };
	for (size_t i = 0; i < 3; i++) {
		vec_push_back(v, &strings[i]);
		assert(strcmp(*(char **)vec_at(v, i), strings[i]) == 0);
	}

	assert(vec_size(v) == 3);

	for (int i = 2; i >= 0; i--) {
		char **popped = (char **)vec_pop_back(v);
		assert(strcmp(*popped, strings[i]) == 0);
	}

	assert(vec_size(v) == 0);
	vec_destroy(v);
	printf("test_push_pop_string passed.\n");
}

void test_insert_erase(void)
{
	vec_t *v = vec_create(TYPE_INT);
	int nums[] = { 10, 20, 30, 40 };

	vec_push_back(v, &nums[0]);
	vec_push_back(v, &nums[2]);
	vec_insert(v, 1, &nums[1]); // Insert 20 at index 1
	assert(vec_size(v) == 3);
	assert(*(int *)vec_at(v, 1) == 20);

	vec_erase(v, 1); // Remove 20
	assert(vec_size(v) == 2);
	assert(*(int *)vec_at(v, 1) == 30);

	vec_destroy(v);
	printf("test_insert_erase passed.\n");
}

void test_nested_vectors(void)
{
	vec_t *outer = vec_create(TYPE_VEC);

	vec_t *inner1 = vec_create(TYPE_INT);
	vec_t *inner2 = vec_create(TYPE_INT);
	int nums[] = { 1, 2, 3 };

	for (size_t i = 0; i < 3; i++) {
		vec_push_back(inner1, &nums[i]);
		vec_push_back(inner2, &nums[2 - i]); // Reverse order
	}

	vec_push_back(outer, inner1);
	vec_push_back(outer, inner2);

	vec_t *retrieved_inner1 = (vec_t *)vec_at(outer, 0);
	vec_t *retrieved_inner2 = (vec_t *)vec_at(outer, 1);

	for (size_t i = 0; i < 3; i++) {
		assert(*(int *)vec_at(retrieved_inner1, i) == nums[i]);
		assert(*(int *)vec_at(retrieved_inner2, i) == nums[2 - i]);
	}

	vec_destroy(outer);
	printf("test_nested_vectors passed.\n");
}

void test_print(void)
{
	vec_t *v = vec_create(TYPE_INT);
	int nums[] = { 10, 20, 30 };

	for (size_t i = 0; i < 3; i++) {
		vec_push_back(v, &nums[i]);
	}

	printf("Expected: Vec: [10, 20, 30]\nActual:   ");
	vec_print(v);

	vec_destroy(v);
	printf("test_print passed.\n");
}

void test_copy(void)
{
	vec_t *v = vec_create(TYPE_INT);
	int nums[] = { 10, 20, 30 };

	for (size_t i = 0; i < 3; i++) {
		vec_push_back(v, &nums[i]);
	}

	vec_t *copy = vec_copy(v);
	assert(vec_size(copy) == vec_size(v));
	for (size_t i = 0; i < vec_size(v); i++) {
		assert(*(int *)vec_at(copy, i) == *(int *)vec_at(v, i));
	}

	vec_destroy(v);
	vec_destroy(copy);
	printf("test_copy passed.\n");
}

int main(void)
{
	test_create_destroy();
	test_push_pop_int();
	test_push_pop_string();
	test_insert_erase();
	test_nested_vectors();
	test_print();
	test_copy();

	printf("All tests passed.\n");
	return 0;
}
