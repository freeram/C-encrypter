#include "filereader.h"

uint8_t *read_stdin_data(size_t *buffer_size)
{
    size_t capacity = INITIAL_CAPACITY;
    size_t size = 0; // Current size of the data in the buffer
    uint8_t *buffer = malloc(capacity);
    int ch;

    if (!buffer)
    {
        perror("Malloc failed in read_stdin_data");
        return NULL;
    }

    // Read byte by byte from stdin
    while ((ch = getchar()) != EOF)
    {
        if (size >= capacity)
        {
            // Increase buffer size when capacity is exceeded
            capacity *= 2;
            uint8_t *new_buffer = realloc(buffer, capacity);
            if (!new_buffer)
            {
                perror("Realloc failed in read_stdin_data: increasing buffer size");
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[size++] = (uint8_t)ch;
    }

    // Getting rid of 0x0a at end of file (linefeed)
    if (size > 0 && buffer[size-1] == '\n')
    {
        size--;
    }

    // Resize the buffer to the actual data size
    uint8_t *final_buffer = realloc(buffer, size);
    if (!final_buffer)
    {
        free(buffer);
        perror("Realloc failed in read_stdin_data: resizing buffer to actual data size");
        return NULL;
    }

    *buffer_size = size; // Write the actual size of the data
    return final_buffer;
}

uint8_t *read_file_data(const char *filename, size_t *buffer_size)
{
    FILE *file;
    uint8_t *buffer;
    size_t size;

    // Open the file for reading
    file = fopen(filename, "rb"); // "rb" for reading binary data
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    // Seek to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET); // Rewind to the start of the file

    // Allocate memory for the buffer to hold the file data
    buffer = (uint8_t *)malloc(size);
    if (buffer == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read the file into the buffer
    if (fread(buffer, 1, size, file) != size)
    {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    // Close the file
    fclose(file);

    // Getting rid of 0x0a at end of file (linefeed)
    if (size > 0 && buffer[size-1] == 0x0a)
    {
        size--;
        uint8_t* new_buffer = realloc(buffer, size);
        if (!new_buffer)
        {
            free(buffer);
            perror("Realloc failed in read_file_data");
            return NULL;
        }
        buffer = new_buffer;
    }

    // Write the size of the data to buffer_size
    *buffer_size = size;

    return buffer;
}

int write_file_data(const char *filename, const char *data)
{
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1; // failure
    }

    // Write data to the file
    if (fprintf(file, "%s", data) < 0)
    {
        perror("Error writing to file");
        fclose(file);
        return 1; // failure
    }

    // Close the file and return 1 to indicate success
    fclose(file);
    return 0;
}
