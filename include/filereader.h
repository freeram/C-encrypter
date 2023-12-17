#ifndef FILE_READER_H
#define FILE_READER_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1024

// Reads data from a file and returns a pointer to the data buffer.
// The size of the data is stored in 'data_size'.
uint8_t* read_file_data(const char* filename, size_t* buffer_size);

// Reads data from stdin and returns a pointer to the data buffer.
// The size of the data is stored in 'data_size'.
uint8_t* read_stdin_data(size_t* buffer_size);

#endif
