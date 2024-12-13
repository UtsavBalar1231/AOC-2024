#include "../helpers/helpers.h"
#include "../helpers/vec.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>

int count_lines(const char *str)
{
	int count = 0;
	int is_empty_line = 1;

	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			if (!is_empty_line)
				count++;
			is_empty_line = 1;
		} else if (str[i] != ' ' && str[i] != '\t') {
			is_empty_line = 0;
		}
	}

	if (!is_empty_line)
		count++;

	return count;
}

__attribute_maybe_unused__ void solve_first_half(char *f_content)
{
	int f_lines = count_lines(f_content);

	char *token = NULL;
	int num_unsafe = 0;

	while ((token = strsep(&f_content, "\n")) != NULL) {
		printf("TOKEN: %s\n", token);
		char *prev_tok = strsep(&token, " ");
		char *next_tok = NULL;
		int increasing = 0;
		int decreasing = 0;

		while ((next_tok = strsep(&token, " ")) != NULL) {
			if (atoi(prev_tok) > atoi(next_tok)) {
				if (decreasing) {
					num_unsafe++;
					printf("ERROR: should be increasing: %s %s\n",
					       prev_tok, next_tok);
					break;
				}

				if (!increasing)
					increasing = 1;

			} else if (atoi(prev_tok) < atoi(next_tok)) {
				if (increasing) {
					printf("ERROR: should be decreasing: %s %s\n",
					       prev_tok, next_tok);
					num_unsafe++;
					break;
				}

				if (!decreasing)
					decreasing = 1;
			}

			int diff = atoi(prev_tok) - atoi(next_tok);

			if (diff > 3 || diff < -3 || diff == 0) {
				printf("ERROR: left: %s, right: %s, diff: %d\n",
				       prev_tok, next_tok, diff);
				num_unsafe++;
				break;
			}

			prev_tok = next_tok;
		}
	}

	printf("Safes: %d\n", f_lines - num_unsafe);
}

void print_char(const void *item)
{
	printf("%c", *(char *)item);
}

void print_inner_vec(const void *item)
{
	vec_t *v = (vec_t *)item;
	vec_print(v, print_char);
}

__attribute_maybe_unused__ void solve_second_half(char *f_content)
{
	int f_lines = count_lines(f_content);

	char *token = NULL;
	int num_unsafe = 0;
	vec_t vec = vec_new(sizeof(vec_t));

	while ((token = strsep(&f_content, "\n")) != NULL) {
		char *next_tok = NULL;

		if (strlen(token) == 0)
			continue;

		printf("TOKEN: %s, len: %lu\n", token, strlen(token));

		vec_t inner_vec = vec_new(sizeof(char *));

		while ((next_tok = strsep(&token, " ")) != NULL) {
			if (strlen(next_tok) == 0)
				continue;

			vec_push(&inner_vec, next_tok);
		}

		vec_push(&vec, &inner_vec);
	}

	printf("TOKENS size: %zu\n", vec_size(&vec));
	vec_print(&vec, print_inner_vec);

	printf("Safes: %d\n", f_lines - num_unsafe);

	vec_free(&vec);
}

int main(void)
{
	char *f_content = NULL;
	/* char *f_content = "A B C\nD E F\nG H I\n";  // Sample content */
	int ret = 0;

	ret = read_file("data.test", &f_content);
	if (ret < 0) {
		perror("Failed to read file");
		return 1;
	}

	/* solve_first_half(f_content); */
	solve_second_half(f_content);
	free(f_content);

	return 0;
}
