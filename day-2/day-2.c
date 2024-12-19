#include "../helpers/helpers.h"
#include "../helpers/vec.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>

int issafe(vec_t *levels)
{
	if (vec_size(levels) < 2)
		return 0;

	int *first = (int *)vec_at(levels, 0);
	int *second = (int *)vec_at(levels, 1);
	/* printf("First: %d, Second: %d\n", *first, *second); */
	int increasing = (*second > *first);

	for (size_t i = 1; i < vec_size(levels); ++i) {
		int *cur = (int *)vec_at(levels, i);
		int *prev = (int *)vec_at(levels, i - 1);

		int abs_diff = abs(*cur - *prev);

		if (abs_diff < 1 || abs_diff > 3 ||
		    (*cur > *prev) != increasing) {
			/* printf("ERROR: %d %d %d\n", *prev, *cur, abs_diff); */
			return 0;
		}
	}

	return 1;
}

int issafe_with_dampener(vec_t *levels)
{
	for (size_t i = 0; i < vec_size(levels); ++i) {
		vec_t *modified = vec_create(TYPE_INT);

		for (size_t j = 0; j < vec_size(levels); ++j) {
			if (j != i) {
				int value = *(int *)vec_at(levels, j);

				vec_push_back(modified, &value);
			}
		}

		if (issafe(modified)) {
			vec_destroy(modified);
			return 1;
		}

		vec_destroy(modified);
	}

	return 0;
}

void solve_second_half(char *f_content)
{
	int num_safe = 0;
	/* vec_t *vec = vec_create(TYPE_VEC); */

	char *content = strdup(f_content);
	if (!content) {
		perror("ERROR: Failed to allocate memory for content copy");
		return;
	}

	char *line = content;
	while ((line = strsep(&content, "\n")) != NULL) {
		if (strlen(line) == 0)
			continue;

		vec_t *levels = vec_create(TYPE_INT);

		char *tok = strsep(&line, " ");
		while (tok) {
			if (strlen(tok) == 0)
				continue;

			int num = atoi(tok);
			vec_push_back(levels, &num);

			tok = strsep(&line, " ");
		}

		if (issafe(levels) || issafe_with_dampener(levels))
			num_safe++;
		/* vec_push_back(vec, levels); */
	}

	/* vec_print(vec); */

	printf("Safes: %d\n", num_safe);

	/* vec_destroy(vec); */
}

void solve_first_half(char *f_content)
{
	int num_safe = 0;

	char *content = strdup(f_content);
	if (!content) {
		perror("ERROR: Failed to allocate memory for content copy");
		return;
	}

	char *line = content;
	while ((line = strsep(&content, "\n")) != NULL) {
		if (strlen(line) == 0)
			continue;

		vec_t *levels = vec_create(TYPE_INT);

		char *tok = strsep(&line, " ");
		while (tok) {
			if (isdigit(tok[0]) ||
			    (tok[0] == '-' && isdigit(tok[1]))) {
				int num = atoi(tok);
				vec_push_back(levels, &num);
			}

			tok = strsep(&line, " ");
		}

		if (issafe(levels))
			num_safe++;
	}

	printf("Safes: %d\n", num_safe);
}

int main(void)
{
	char *f_content = NULL;
	int ret = 0;

	ret = read_file("data.input", &f_content);
	if (ret < 0) {
		perror("Failed to read file");
		return 1;
	}

	solve_first_half(f_content);
	solve_second_half(f_content);

	free(f_content);

	return 0;
}
