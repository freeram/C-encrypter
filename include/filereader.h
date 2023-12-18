#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Initial buffer capacity for read_stdin_data and read_file_data
// Buffer gets resized to exact needed size by end of execution
#define INITIAL_CAPACITY 1024

// Reads data from stdin and returns a pointer to the data buffer.
// The size of the data is stored in 'buffer_size'.
uint8_t *read_stdin_data(size_t *buffer_size);

// Reads data from a file and returns a pointer to the data buffer.
// The size of the data is stored in 'buffer_size'.
uint8_t *read_file_data(const char *filename, size_t *buffer_size);

// Writes data to a file and returns 1 if successful
int write_file_data(const char *filename, const char *data);

#endif
