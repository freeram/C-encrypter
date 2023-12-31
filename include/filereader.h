#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

// Initial buffer capacity for read_stdin_data and read_file_data
// Buffer gets resized to exact needed size by end of execution
#define INITIAL_CAPACITY 1024

// Reads data from stdin and returns a pointer to a dynamically allocated buffer of uint8_t.
// The size of the data is stored in 'buffer_size'.
// Returns NULL on error
uint8_t *read_stdin_data_uint8t(size_t *buffer_size);

// Reads data from stdin and returns a pointer to a dynamically allocated buffer of chars.
// The size of the buffer is stored in 'buffer_size'.
// Returns NULL on error
char *read_stdin_data_char(size_t *buffer_size);

// Reads data from stdin in HEX and returns a pointer to a dynamically allocated buffer of uint8_t.
// The size of the data is stored in 'buffer_size'.
// Returns NULL on error (e.g. invalid HEX)
uint8_t *read_stdin_data_hex(size_t *buffer_size);

// Reads data from a file and returns a pointer to a dynamically allocated buffer of uint8_t.
// The size of the data is stored in 'buffer_size'.
// Returns NULL on error
uint8_t *read_file_data(const char *filename, size_t *buffer_size);

#endif
