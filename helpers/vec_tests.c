#include "vec.h"
#include <stdio.h>
#include <assert.h>

int main(void)
{
	// 1. Test vec_new
	vec_t v = vec_new(TYPE_INT);
	assert(vec_size(&v) == 0);
	assert(vec_capacity(&v) == 8);

	// 2. Test vec_push
	for (int i = 0; i < 8; i++) {
		vec_push(&v, &i);
	}
	assert(vec_size(&v) == 8);
	assert(vec_capacity(&v) == 8);

	// Push to trigger reallocation
	int val = 8;
	vec_push(&v, &val);
	assert(vec_size(&v) == 9);
	assert(vec_capacity(&v) == 16);

	// Print vector
	printf("After pushing elements: ");
	vec_print(&v);

	// 3. Test vec_at
	for (int i = 0; i < 9; i++) {
		int *item = vec_at(&v, i);
		assert(*item == i);
	}

	// 4. Test vec_pop
	for (int i = 8; i >= 0; i--) {
		int *item = vec_pop(&v);
		assert(*item == i);
		assert(vec_size(&v) == (size_t)i);
	}
	assert(vec_size(&v) == 0);

	// 5. Test vec_insert
	for (int i = 0; i < 5; i++) {
		vec_push(&v, &i);
	}
	int val_insert = 99;
	vec_insert(&v, 2, &val_insert);
	assert(vec_size(&v) == 6);
	int *item = vec_at(&v, 2);
	assert(*item == 99);

	printf("After inserting 99 at index 2: ");
	vec_print(&v);

	// 6. Test vec_remove
	vec_remove(&v, 2);
	assert(vec_size(&v) == 5);
	item = vec_at(&v, 2);
	assert(*item == 2);

	printf("After removing element at index 2: ");
	vec_print(&v);

	// 7. Test boundary conditions
	// Insert at the beginning
	int val_begin = 77;
	vec_insert(&v, 0, &val_begin);
	assert(vec_size(&v) == 6);
	item = vec_at(&v, 0);
	assert(*item == 77);

	printf("After inserting 77 at the beginning: ");
	vec_print(&v);

	// Insert at the end
	int val_end = 88;
	vec_insert(&v, vec_size(&v), &val_end);
	assert(vec_size(&v) == 7);
	item = vec_at(&v, vec_size(&v) - 1);
	assert(*item == 88);

	printf("After inserting 88 at the end: ");
	vec_print(&v);

	// Remove from the beginning
	vec_remove(&v, 0);
	assert(vec_size(&v) == 6);
	item = vec_at(&v, 0);
	assert(*item == 0);

	printf("After removing element at the beginning: ");
	vec_print(&v);

	// Remove from the end
	vec_remove(&v, vec_size(&v) - 1);
	assert(vec_size(&v) == 5);

	printf("After removing element at the end: ");
	vec_print(&v);

	// 8. Test vec_free
	vec_free(&v);
	assert(vec_size(&v) == 0);
	assert(vec_capacity(&v) == 0);
	assert(v.data == NULL);

	// 9. Check different types
	vec_t v_char = vec_new(TYPE_CHAR);
	vec_t v_float = vec_new(TYPE_FLOAT);

	char c = 'a';
	float f = 3.14;

	vec_push(&v_char, &c);
	vec_push(&v_float, &f);

	assert(*(char *)vec_at(&v_char, 0) == 'a');
	assert(*(float *)vec_at(&v_float, 0) == 3.14f);
	vec_free(&v_char);
	vec_free(&v_float);

	// 10. Nested vectors
	vec_t v_outer = vec_new(TYPE_VEC);

	for (int i = 0; i < 3; i++) {
		vec_t v_inner = vec_new(TYPE_INT);
		for (int j = 0; j < 3; j++) {
			vec_push(&v_inner, &j);
		}
		vec_push(&v_outer, &v_inner);
	}

	for (int i = 0; i < 3; i++) {
		vec_t *inner = (vec_t *)vec_at(&v_outer, i);
		for (int j = 0; j < 3; j++) {
			int *val = (int *)vec_at(inner, j);
			assert(*val == j);
		}
	}
	vec_free(&v_outer);

	// 11. Large number of vectors
	vec_t v11 = vec_new(TYPE_INT);
	size_t large_size = 10000;
	for (size_t i = 0; i < large_size; i++) {
		vec_push(&v11, &i);
	}
	assert(vec_size(&v11) == large_size);

	for (int i = 0; i < (int)large_size; i++) {
		assert(*(int *)vec_at(&v11, i) == i);
	}

	vec_free(&v11);

	// 12. Edge Cases in vec_at
	vec_t v12 = vec_new(TYPE_INT);
	val = 42;

	vec_push(&v12, &val);
	assert(*(int *)vec_at(&v12, 0) == 42);

	vec_push(&v12, &val);
	assert(*(int *)vec_at(&v12, 1) == 42);

	vec_free(&v12);

	printf("All tests passed!\n");

	return 0;
}
