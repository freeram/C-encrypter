#include "padding.h"

int pad_buffer_pkcs5(uint8_t **buffer, size_t old_len, size_t new_len)
{
    uint8_t *new_buffer = realloc(*buffer, new_len);

    if (!new_buffer)
    {
        perror("Realloc failed in pad_buffer_pkcs5");
        return 1;
    }

    // Initialize the new portion of the buffer according to amount of bytes padded
    // e.g. if 3 bytes must be padded, fill each with 0x03
    memset(new_buffer + old_len, new_len - old_len, new_len - old_len);

    *buffer = new_buffer;

    return 0;
}

int depad_buffer_pkcs5(uint8_t **buffer, size_t *buffer_size)
{
    if (!buffer || !*buffer || !buffer_size || *buffer_size == 0)
    {
        perror("Invalid input in depad_buffer_pkcs5");
        return 1;
    }

    size_t size = *buffer_size;
    uint8_t padding_value = (*buffer)[size - 1]; // Get the last byte (padding value)

    // Validate padding value
    if (padding_value == 0 || padding_value > size)
    {
        perror("Invalid padding in depad_buffer_pkcs5");
        return 1;
    }

    // Check if all padding bytes have the same value
    for (size_t i = size - padding_value; i < size; i++)
    {
        if ((*buffer)[i] != padding_value)
        {
            perror("Padding bytes do not have same value in depad_buffer_pkcs5");
            return 1;
        }
    }

    // Adjust buffer size to remove padding
    *buffer_size = size - padding_value;

    return 0;
}
