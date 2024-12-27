#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "../helpers/helpers.h"
#include <stdlib.h>
#include <string.h>

typedef enum part { PART_ONE, PART_TWO } part_t;

int parse_tok(const char *token)
{
	if (strlen(token) < 3)
		return 0;

	if (!isdigit(token[0]))
		return 0;

	const char *tmp_token = strndup(token, strlen(token));

	int i = 0;
	while (isdigit(token[i]))
		i++;

	char *first_number = malloc(sizeof(char *) * i);
	strncpy(first_number, tmp_token, i);

	if (token[i] != ',')
		return 0;

	int j = i + 1;
	if (!isdigit(token[j]))
		return 0;

	while (isdigit(token[j]))
		j++;

	if (token[j] != ')')
		return 0;

	j--;
	j--;

	int k = i + 1;
	while (k--)
		tmp_token++;

	char *second_number = malloc(sizeof(char *) * j - (i - 1));
	strncpy(second_number, tmp_token, j - (i - 1));

	/* printf("%s * %s\n", first_number, second_number); */
	return (strtoul(first_number, NULL, 10) *
		strtoul(second_number, NULL, 10));
}

int part(char *f_content, part_t p)
{
	int result = 0;
	char *rem;
	char *token;
	char *delim = "mul(";
	int enabled = 1;

	token = strsplit_r(f_content, delim, &rem);
	if (strstr(token, "don\'t()") != NULL)
		enabled = 0;

	while (token != NULL) {
		if (strlen(token) < 4)
			goto skip;

		int ret = 0;

		switch (p) {
		case PART_ONE:
			ret = parse_tok(token);
			if (!ret)
				goto skip;
			result += ret;
			break;
		case PART_TWO:
			ret = parse_tok(token);
			if (!ret)
				goto skip;

			if (enabled) {
				printf("ENABLED TOK: %s\tret: %d\n", token,
				       ret);
				result += ret;
			} else {
				printf("DISABLED TOK: %s\n", token);
			}

			if (strstr(token, "do()") != NULL) {
				printf("ENABLED from token: %s\n", token);
				enabled = 1;
			}
			if (strstr(token, "don\'t()") != NULL) {
				printf("DISABLED from token: %s\n", token);
				enabled = 0;
			}
		}
skip:
		token = strsplit_r(rem, delim, &rem);
	}

	return result;
}

int main(void)
{
	// const char *f_name = "data.input";
	// int ret = 0;
	// char *f_content = NULL;

	// ret = read_file(f_name, &f_content);
	// if (ret < 0) {
	// 	perror("ERROR: Failed to read file");
	// 	return 1;
	// }

	const char *f_content = "don't()mul(3,3)mul(4,4)";
	int result = 0;

	/* part(strdup(f_content), PART_ONE); */
	result = part(strdup(f_content), PART_TWO);
	assert(result == 0);
	/* printf("Result: %d\n", result); */

	// free(f_content);
	return 0;
}
