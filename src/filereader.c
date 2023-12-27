#include "filereader.h"

uint8_t *read_stdin_data_uint8t(size_t *buffer_size)
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
    if (size > 0 && buffer[size - 1] == '\n')
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

char *read_stdin_data_char(size_t *buffer_size)
{
    size_t capacity = INITIAL_CAPACITY;
    size_t size = 0; // Current size of the data in the buffer
    char *buffer = malloc(capacity);
    int ch;

    if (!buffer)
    {
        perror("Malloc failed in read_stdin_data_char");
        return NULL;
    }

    // Read character by character from stdin
    while ((ch = getchar()) != EOF)
    {
        if (size >= capacity)
        {
            // Increase buffer size when capacity is exceeded
            capacity *= 2;
            char *new_buffer = realloc(buffer, capacity);
            if (!new_buffer)
            {
                perror("Realloc failed in read_stdin_data_char: increasing buffer size");
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[size++] = (char)ch;
    }

    // Remove newline at end of stdin
    if (size > 0 && buffer[size - 1] == '\n')
    {
        size--;
    }

    // Resize the buffer to the actual data size
    char *final_buffer = realloc(buffer, size + 1);
    if (!final_buffer)
    {
        free(buffer);
        perror("Realloc failed in read_stdin_data_char: resizing buffer to actual data size");
        return NULL;
    }

    final_buffer[size] = '\0'; // Null-terminate the string
    *buffer_size = size;       // Write the actual size of the data
    return final_buffer;
}

// Helper function to convert a hex character to its numeric value
static int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

uint8_t *read_stdin_data_hex(size_t *buffer_size)
{
    size_t capacity = INITIAL_CAPACITY;
    size_t size = 0; // Current size of the data in the buffer
    uint8_t *buffer = malloc(capacity);
    int ch1, ch2;

    if (!buffer)
    {
        perror("Malloc failed in read_stdin_data_hex");
        return NULL;
    }

    // Read a character from stdin
    while ((ch1 = getchar()) != EOF)
    {
        // If it's a space, skip
        if (isspace(ch1))
        {
            continue;
        }

        // Read a second character, if it's a space or EOF, invalid input
        // Because "61 6" or "6 61" is not a valid hex sequence
        if (((ch2 = getchar()) == EOF) || isspace(ch2))
        {
            perror("Invalid hexadecimal input");
            free(buffer);
            return NULL;
        }

        if (!isxdigit(ch1) || !isxdigit(ch2))
        {
            perror("Invalid hexadecimal input");
            free(buffer);
            return NULL;
        }

        if (size >= capacity)
        {
            // Increase buffer size when capacity is exceeded
            capacity *= 2;
            uint8_t *new_buffer = realloc(buffer, capacity);
            if (!new_buffer)
            {
                perror("Realloc failed in read_stdin_data_hex: increasing buffer size");
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }

        // Convert two hex characters to one byte
        int byte = (hex2int(ch1) << 4) | hex2int(ch2);
        buffer[size++] = (uint8_t)byte;
    }

    // Resize the buffer to the actual data size
    uint8_t *final_buffer = realloc(buffer, size);
    if (!final_buffer)
    {
        free(buffer);
        perror("Realloc failed in read_stdin_data_hex: resizing buffer to actual data size");
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
    if (size > 0 && buffer[size - 1] == 0x0a)
    {
        size--;
        uint8_t *new_buffer = realloc(buffer, size);
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
