#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int read_file(const char *f_name, char **f_content)
{
	int ret = 0;
	int fd = open(f_name, O_RDONLY);

	if (fd < 0) {
		perror("Failed to read file");
		goto exit;
	}

	struct stat sb;
	if (fstat(fd, &sb) < 0) {
		perror("Failed to get file size");
		ret = -1;
		goto release_fd;
	}

	if (!sb.st_size) { // Handle empty file
		*f_content = malloc(1); // Allocate a minimal buffer
		if (*f_content == NULL) {
			perror("Failed to allocate memory");
			ret = -1;
			goto exit;
		}
		(*f_content)[0] = '\0'; // Null-terminate
		goto exit;
	}

	// Allocate buffer to hold file content
	*f_content = malloc(sb.st_size + 1); // +1 for null terminator
	if (*f_content == NULL) {
		perror("Failed to allocate memory");
		ret = -1;
		goto release_fd;
	}

	char *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (f_content == MAP_FAILED) {
		perror("Failed to map file");
		ret = -1;
		goto free_content;
	}

	memcpy(*f_content, mapped, sb.st_size);
	/* (*f_content)[sb.st_size] = '\0'; */

	munmap(mapped, sb.st_size);
	goto release_fd;

free_content:
	free(*f_content);
release_fd:
	close(fd);
exit:
	return ret;
}

int count_str_lines(const char *str)
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

char *strsplit_r(char *s, const char *delim, char **save_ptr)
{
	if (s == NULL)
		s = *save_ptr;

	if (s == NULL || *s == '\0') {
		*save_ptr = NULL;
		return NULL;
	}

	size_t delim_len = strlen(delim);

	if (delim_len == 0) {
		*save_ptr = s + strlen(s);
		return s;
	}

	if (delim_len == 1) {
		s += strspn(s, delim);
	} else {
		while (strstr(s, delim) == s)
			s += delim_len;
	}

	if (*s == '\0') {
		*save_ptr = NULL;
		return NULL;
	}

	// Find the end of the token
	char *end = strstr(s, delim);

	if (end == NULL) {
		*save_ptr = s + strlen(s);
		return s;
	}

	// Terminate the token and update save_ptr
	*end = '\0';
	*save_ptr = end + delim_len;
	return s;
}
