#ifndef UTILS_H
#define UTILS_H

/**
 * Read the contents of a file into a buffer and return it.
 * @param input_file The path to the input file.
 * @return A pointer to the buffer containing the file contents, or NULL if the file couldn't be opened.
 */
char *read_file(char *input_file);

#endif
