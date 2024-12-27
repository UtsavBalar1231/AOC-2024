#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../helpers/helpers.h"

int compare(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int count_occurances(int *a, int file_length, int key)
{
	int count = 0;
	for (int i = 0; i < file_length; i++) {
		if (a[i] == key)
			count++;
	}
	return count;
}

int main()
{
	char *fcontent = NULL;

	const char *file_name = "./data.input";
	read_file(file_name, &fcontent);
	if (!fcontent) {
		fprintf(stderr, "Error reading %s file", file_name);
		return 1;
	}

	int file_length = count_str_lines(fcontent);

	int *first = (int *)malloc(sizeof(int) * file_length);
	int *second = (int *)malloc(sizeof(int) * file_length);

	int i = 0;
	char *token;

	while ((token = strsep(&fcontent, "\n"))) {
		if (strlen(token) == 0)
			continue;

		// split a<space><space><space>b
		char *token1 = strtok(token, " ");
		char *token2 = strtok(NULL, " ");

		first[i] = atoi(token1);
		second[i] = atoi(token2);

		i++;
	}

	qsort(first, file_length, sizeof(first[0]), compare);
	qsort(second, file_length, sizeof(second[0]), compare);

	i = 0;
	int sum = 0;
	while (i < file_length) {
		/* printf("%d %d\n", first[i], second[i]); */
		int tmp = (int)(first[i] - second[i]);
		sum += abs(tmp);
		i++;
	}

	printf("sum1 = %d\n", sum);

	i = 0;
	sum = 0;
	while (i < file_length) {
		int key = first[i];
		int count = count_occurances(second, file_length, key);
		sum = sum + (count * key);
		i++;
	}

	printf("sum2 = %d\n", sum);

	free(fcontent);
	free(first);
	free(second);

	return 0;
}
