#ifndef FILE_READER_H
#define FILE_READER_H

#include <stddef.h>
#include <stdint.h>

// Reads data from a file and returns a pointer to the data buffer.
// The size of the data is stored in 'data_size'.
uint8_t* read_file_data(const char* filename, size_t* buffer_size);

// Reads data from stdin and returns a pointer to the data buffer.
// The size of the data is stored in 'data_size'.
uint8_t* read_stdin_data(size_t* buffer_size);

#endif
