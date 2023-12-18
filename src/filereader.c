#include "filereader.h"

uint8_t* read_file_data(const char* filename, size_t* buffer_size)
{
    // Implement
}

uint8_t* read_stdin_data(size_t* buffer_size)
{
    size_t capacity = INITIAL_CAPACITY;
    size_t size = 0;  // Current size of the data in the buffer
    uint8_t* buffer = malloc(capacity);
    int ch;

    if (!buffer) {
        perror("Malloc failed in read_stdin_data");
        exit(-1);
    }

    // Read byte by byte from stdin
    while ((ch = getchar()) != EOF) {
        if (size >= capacity) {
            // Increase buffer size when capacity is exceeded
            capacity *= 2;
            uint8_t* new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                perror("Realloc failed in read_stdin_data: increasing buffer size");
                free(buffer);
                exit(-1);
            }
            buffer = new_buffer;
        }
        buffer[size++] = (uint8_t)ch;
    }

    // Blowfish takes 64-bit blocks for input
    // Buffer must be padded until it is a multiple of 64 bits
    while (size % 8 != 0) {
        size++;
    }

    // Resize the buffer to the actual data size
    uint8_t* final_buffer = realloc(buffer, size);
    if (!final_buffer) {
        free(buffer);
        perror("Realloc failed in read_stdin_data: resizing buffer to actual data size");
        exit(-1);
    }

    *buffer_size = size; // Write the actual size of the data
    return final_buffer;
}
