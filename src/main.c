#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "encryption.h"
#include "filereader.h"

extern int optind, opterr, optopt;
extern char *optarg;

void getflags(int argc, char *argv[], int *gflag, int *dflag, char **keyvalue)
{
    int index;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "gdk:")) != -1)
        switch (c)
        {
        case 'g':
            *gflag = 1;
            break;
        case 'd':
            *dflag = 1;
            break;
        case 'k':
            *keyvalue = optarg;
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
        return 1;
    }

    // Initialize the new portion of the buffer according to amount of bytes padded
    // e.g. if 3 bytes must be padded, fill each with 0x03
    memset(new_buffer + old_len, new_len - old_len, new_len - old_len);

    *buffer = new_buffer;

    return 0; // Success
}

int main(int argc, char *argv[])
{
    // Getting flags
    int gflag = 0;         // -g: Generate a key flag
    int dflag = 0;         // -d: Decrypt flag
    char *keyvalue = NULL; // -k: Get key from CLI flag [path to key file]

    getflags(argc, argv, &gflag, &dflag, &keyvalue);

    if (gflag && dflag)
    {
        printf("Error: Cannot generate key and decrypt at the same time. Exiting\n");
        return 1;
    }
    if (gflag && (keyvalue != NULL))
    {
        printf("Error: Cannot generate key and parse key at the same time. Exiting\n");
        return 1;
    }

    uint8_t *key;
    size_t key_len;

    if (keyvalue != NULL)
    {
        key = read_file_data(keyvalue, &key_len);
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

#ifdef DEBUG
    printf("Key:\n");
    for (size_t i = 0; i < key_len; i++)
    {
        printf("%c", key[i]);
    }
    printf("\n");

    printf("Key in hex:\n");
    for (size_t i = 0; i < key_len; i++)
    {
        printf("%02x ", key[i]);
    }
    printf("\n");

    printf("Key len: %ld\n", key_len);
#endif

    blowfish_init(key, key_len);

    size_t buffer_size;
    uint8_t *buffer = read_stdin_data(&buffer_size);

    if (buffer == NULL)
    {
        printf("Error: reading stdin failed. Exiting\n");
        return 1;
    }

    // Blowfish takes 64-bit blocks for input
    // Buffer must be padded with PKCS#5 until it is a multiple of 128 bits
    if (buffer_size % 16 != 0)
    {
        size_t old_size = buffer_size;
        while (buffer_size % 16 != 0)
        {
            buffer_size++;
        }
        if (pad_buffer_pkcs5(&buffer, old_size, buffer_size))
        {
            printf("Error: realloc failure in pad_buffer. Exiting\n");
            return 1;
        }
    }

#ifdef DEBUG
    printf("Buffer size: %ld ; Buffer size in bits: %ld\n", buffer_size, buffer_size * 8);

    printf("Original buffer:\n");
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    printf("Encrypted buffer:\n");
#endif

    encrypt(buffer, buffer_size);

    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

#ifdef DEBUG
    decrypt(buffer, buffer_size);

    printf("Decrypted buffer:\n");
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
#endif

    free(buffer);
    free(key);

    return 0;
}
