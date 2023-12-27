#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "encryption.h"
#include "filereader.h"
#include "encoding.h"

extern int optind, opterr, optopt;
extern char *optarg;

void getflags(int argc, char *argv[], int *dflag, int *hflag, char **keydir)
{
    int index;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "dhk:")) != -1)
        switch (c)
        {
        case 'd':
            *dflag = 1;
            break;
        case 'h':
            *hflag = 1;
            break;
        case 'k':
            *keydir = optarg;
            break;
        case '?':
            if (optopt == 'k')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            exit(1);
        default:
            abort();
        }

    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}

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

int main(int argc, char *argv[])
{
    //// Getting flags
    int dflag = 0;       // -d: Decrypt flag
    int hflag = 0;       // -h: Input/Output in hex flag [except key]
    char *keydir = NULL; // -k: Get keydir from CLI flag [path to key file]

    getflags(argc, argv, &dflag, &hflag, &keydir);
    //// (/Getting flags)

    //// Getting the key
    uint8_t *key;
    size_t key_len;

    if (keydir != NULL)
    {
        key = read_file_data(keydir, &key_len);
    }
    else
    {
        key = read_file_data("key.txt", &key_len);
    }

    if (key == NULL)
    {
        printf("Error: reading key from file failed. Exiting\n");
        return 1;
    }

    // Blowfish takes keys that are a minimum of 32 bits and a maximum of 448 bits
    if (key_len < 4)
    {
        printf("Key length must be at least 4 characters. Exiting\n");
        return 1;
    }
    if (key_len > 56)
    {
        printf("Key length must not exceed 56 characters. Exiting\n");
        return 1;
    }
    //// (/Getting the key)

    //// Initializing blowfish
    blowfish_init(key, key_len);
    //// (/Initializing blowfish)

    //// Getting input
    uint8_t *buffer = NULL;
    size_t buffer_size;

    // Input read in hex
    if (hflag)
    {
        buffer = read_stdin_data_hex(&buffer_size);
    }
    // Input for decryption read in chars, decoded from base64
    else if (dflag)
    {
        char *base64 = NULL;
        base64 = read_stdin_data_char(&buffer_size);
        if (base64 == NULL)
        {
            printf("Error: reading stdin chars failed. Exiting\n");
            return 1;
        }
        buffer = base64_decode(base64, &buffer_size);
        free(base64);
    }
    // Input for encryption read in uint8_t
    else
    {
        buffer = read_stdin_data_uint8t(&buffer_size);
    }

    if (buffer == NULL)
    {
        printf("Error: reading stdin failed. Exiting\n");
        return 1;
    }

    // Blowfish takes 64-bit blocks for input, we cannot decrypt otherwise
    if (buffer_size % 8 != 0 && dflag)
    {
        printf("Input for blowfish decryption is not a multiple of 64 bits. Exiting\n");
        return 1;
    }

    // If we're encrypting
    // Buffer must be padded with PKCS#5 until it is a multiple of 128 bits
    if (buffer_size % 16 != 0 && !dflag)
    {
        size_t old_size = buffer_size;
        while (buffer_size % 16 != 0)
        {
            buffer_size++;
        }
        if (pad_buffer_pkcs5(&buffer, old_size, buffer_size))
        {
            printf("Error: padding the buffer failed. Exiting\n");
            return 1;
        }
    }
    //// (/Getting input)

    //// Performing encryption/decryption
    if (dflag) // Decrypt flag
    {
        decrypt(buffer, buffer_size);
    }
    else
    {
        encrypt(buffer, buffer_size);
    }
    //// (/Performing encryption/decryption)

    //// Printing output

    // Our buffer is now in HEX
    // This outputs hex and quits (on hex flag)
    if (hflag)
    {
        for (size_t i = 0; i < buffer_size; i++)
        {
            printf("%02x ", buffer[i]);
        }
        printf("\n");

        free(buffer);
        free(key);

        return 0;
    }
    // This outputs ASCII and quits (on decrypt flag)
    else if (dflag)
    {
        // Remove padding bytes
        if (depad_buffer_pkcs5(&buffer, &buffer_size))
        {
            printf("Error: depadding the buffer failed. Exiting\n");
            free(buffer);
            free(key);
            return 1;
        }

        for (size_t i = 0; i < buffer_size; i++)
        {
            printf("%c", buffer[i]);
        }
        printf("\n");

        free(buffer);
        free(key);

        return 0;
    }

    // This gets a base64 representation of our buffer
    char *base64 = NULL;
    size_t base64_len;

    base64 = base64_encode(buffer, buffer_size, &base64_len);

    if (base64 == NULL)
    {
        printf("Error: encoding base64 failed. Exiting\n");
        free(buffer);
        free(key);
        return 1;
    }

    // Printing base64 chars
    for (size_t i = 0; i < base64_len; i++)
    {
        printf("%c", base64[i]);
    }
    printf("\n");

    free(buffer);
    free(key);
    free(base64);

    //// (/Printing output)

    return 0;
}
