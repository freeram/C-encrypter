#include "encoding.h"

static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const uint8_t *buffer, size_t buffer_size, size_t *output_length)
{
    *output_length = 4 * ((buffer_size + 2) / 3);
    uint8_t *encoded = malloc(*output_length + 1); // +1 for null terminator

    if (encoded == NULL)
    {
        perror("Malloc failed in base64_encode");
        return NULL;
    }

    for (size_t i = 0, j = 0; i < buffer_size;)
    {
        uint32_t octet_a = i < buffer_size ? buffer[i++] : 0;
        uint32_t octet_b = i < buffer_size ? buffer[i++] : 0;
        uint32_t octet_c = i < buffer_size ? buffer[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded[j++] = base64_chars[(triple >> 3 * 6) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 2 * 6) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 1 * 6) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 0 * 6) & 0x3F];
    }

    for (size_t i = 0; i < (3 - buffer_size % 3) % 3; i++)
    {
        encoded[*output_length - 1 - i] = '=';
    }

    encoded[*output_length] = '\0';
    return (char *)encoded;
}

static int base64_char_value(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    return -1;
}

uint8_t *base64_decode(const char *input, size_t *output_length)
{
    size_t input_length = strlen(input);
    if (input_length % 4 != 0)
    {
        perror("Invalid input length in base64_decode");
        return NULL;
    }

    *output_length = input_length / 4 * 3;
    if (input[input_length - 1] == '=')
        (*output_length)--;
    if (input[input_length - 2] == '=')
        (*output_length)--;

    uint8_t *decoded = malloc(*output_length);
    if (decoded == NULL)
    {
        perror("Malloc failed in base64_decode");
        return NULL;
    }

    for (size_t i = 0, j = 0; i < input_length;)
    {
        int sextet_a = base64_char_value(input[i]);
        int sextet_b = base64_char_value(input[i + 1]);
        int sextet_c = (input[i + 2] != '=') ? base64_char_value(input[i + 2]) : 0;
        int sextet_d = (input[i + 3] != '=') ? base64_char_value(input[i + 3]) : 0;
        i += 4;

        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

        if (j < *output_length)
            decoded[j++] = (triple >> 16) & 0xFF;
        if (i < input_length - 4 || input[i - 2] != '=')
            decoded[j++] = (triple >> 8) & 0xFF;
        if (i < input_length - 4 || input[i - 1] != '=')
            decoded[j++] = triple & 0xFF;
    }

    return decoded;
}
