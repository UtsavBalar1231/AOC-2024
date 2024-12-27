#ifndef _HELPERS_H_
#define _HELPERS_H_

/**
 * @brief Reads the contents of a file into a dynamically allocated buffer.
 *
 * This function opens a file, determines its size, allocates a buffer to hold the entire file content 
 * (plus a null terminator), reads the file into the buffer using memory mapping for efficiency, 
 * and null-terminates the buffer.
 *
 * @param f_name   The name of the file to read.
 * @param f_content A pointer to a char pointer where the file content will be stored. The function
 *                  allocates memory for the content, and the caller is responsible for freeing
 *                  this memory using `free()`.
 *
 * @return 0 on success, -1 on failure.
 *
 * @details
 * The function handles errors at each step:
 *   - Opening the file.
 *   - Getting the file size.
 *   - Allocating memory for the file content.
 *   - Mapping the file into memory
 *   - Handling empty file by allocating a minimal buffer and null terminate it.
 * 
 * The function uses `mmap` for efficient file reading, especially for large files. It maps the file
 * into memory and then copies the content to the allocated buffer.
 *
 * @note
 * - The caller is responsible for freeing the memory allocated for `*f_content`.
 * - The function adds a null terminator (`\0`) at the end of the allocated buffer, making it a valid
 *   C-style string.
 * - Error messages are printed to `stderr` using `perror` in case of failures.
 *
 * @code{.c}
 * #include <stdio.h>
 * #include <stdlib.h>
 *
 * // ... (Include the read_file function here) ...
 *
 * int main() {
 *     char *file_content;
 *     if (read_file("my_file.txt", &file_content) == 0) {
 *         printf("File content:\n%s\n", file_content);
 *         free(file_content);
 *     } else {
 *         fprintf(stderr, "Error reading file\n");
 *     }
 *     return 0;
 * }
 * @endcode
 */
int read_file(const char *f_name, char **f_content);

/**
 * @brief Counts the number of lines in a string.
 *
 * This function iterates through a null-terminated string and counts the number of lines. A line is
 * defined as a sequence of characters terminated by a newline character ('\n'). Empty lines (lines
 * containing only whitespace characters like space and tab) are not counted. The function also handles
 * the case where the last line in the string does not end with a newline character.
 *
 * @param str The null-terminated string to count lines in.
 *
 * @return The number of lines in the string.
 *
 * @details
 * - The function considers a line to be non-empty if it contains at least one non-whitespace character 
 *   before a newline or the end of the string.
 * - If the string is empty or only contains whitespace characters, the function returns 0.
 *
 * @code{.c}
 * #include <stdio.h>
 *
 * // ... (Include the count_str_lines function here) ...
 *
 * int main() {
 *     char str1[] = "Line 1\nLine 2\nLine 3";
 *     char str2[] = "Line 1\n\nLine 3"; // Empty line in the middle
 *     char str3[] = "  \t  "; // Only whitespace
 *     char str4[] = ""; // Empty string
 *
 *     printf("Lines in str1: %d\n", count_str_lines(str1)); // Output: 3
 *     printf("Lines in str2: %d\n", count_str_lines(str2)); // Output: 2
 *     printf("Lines in str3: %d\n", count_str_lines(str3)); // Output: 0
 *     printf("Lines in str4: %d\n", count_str_lines(str4)); // Output: 0
 *
 *     return 0;
 * }
 * @endcode
 */
int count_str_lines(const char *str);

/**
 *
 * @brief A reentrant string tokenizer that supports multi-character delimiters.
 *
 * This function breaks a string into a sequence of tokens based on a specified delimiter string.
 * It is a reentrant alternative to `strtok` and `strtok_r` that treats the delimiter as a literal
 * string rather than a set of individual characters.
 * 
 * @param s       The string to be tokenized. On the first call, this should be the string you want to
 *                break into tokens. On subsequent calls for the same string, it should be `NULL`.
 * @param delim   The delimiter string. This string specifies the characters or sequence of characters 
 *                that separate tokens.
 * @param save_ptr A pointer to a `char *` variable. This pointer is used internally by the function to
 *                store its current position in the string being parsed. You must provide the same
 *                `save_ptr` across all calls to `strsplit_r` when tokenizing the same string.
 *
 * @return On success, a pointer to the beginning of the next token found in the string.
 *         If no more tokens are found, or if `s` is `NULL` or an empty string (`""`) on the initial call, 
 *         the function returns `NULL`.
 *
 * @details
 * **Reentrancy:**
 * The `strsplit_r` function is reentrant, making it safe for use in multi-threaded programs. It 
 * achieves reentrancy by using the `save_ptr` argument to store its state instead of relying on static 
 * variables.
 *
 * **Behavior:**
 * 1. **Initialization:** On the first call to `strsplit_r` for a new string, pass the string to be
 *    tokenized in the `s` parameter.
 * 2. **Subsequent Calls:** On subsequent calls to tokenize the same string, pass `NULL` as the `s`
 *    parameter. The function uses the `save_ptr` to continue parsing from where it left off.
 * 3. **Delimiter Handling:** The `delim` parameter is treated as a complete delimiter string. For 
 *    example, if `delim` is "::", the function splits the string at each occurrence of "::".
 *    - **Empty Delimiter:** If `delim` is an empty string (`""`), the function returns the entire 
 *      remaining string as a single token.
 *    - **Single Character Delimiter Optimizations:** When `delim` is a single character string, the 
 *      function utilizes optimized functions `strspn` and potentially `strcspn` for improved 
 *      performance.
 * 4. **Leading Delimiters:** Leading delimiters are skipped. If the string starts with the delimiter,
 *    the function moves past them before returning the first token.
 * 5. **Consecutive Delimiters:** Consecutive delimiters are treated as a single delimiter.
 * 6. **Token Termination:** Each token returned by the function is automatically null-terminated by 
 *    replacing the delimiter with a null character (`\0`).
 * 7. **End of String:** When no more tokens are found, `strsplit_r` returns `NULL`. The value of 
 *    `*save_ptr` is also set to `NULL`.
 *
 * @note 
 * - The function modifies the original string `s` by inserting null terminators (`\0`) to mark the end 
 *   of each token.
 * - The user is responsible for managing the memory allocated for the input string `s`.
 *
 * @code{.c}
 * #include <stdio.h>
 * #include <string.h>
 * #include <assert.h>
 *
 * // (Include the strsplit_r function here)
 *
 * int main() {
 *     char str[] = "::hello::world::again::";
 *     char *saveptr;
 *     char *token;
 *
 *     token = strsplit_r(str, "::", &saveptr);
 *     while (token != NULL) {
 *         printf("Token: %s\n", token);
 *         token = strsplit_r(NULL, "::", &saveptr);
 *     }
 *
 *     return 0;
 * }
 * @endcode
 *
 * @see strtok, strtok_r
 */
char *strsplit_r(char *s, const char *delim, char **save_ptr);

#endif // _HELPERS_H_
