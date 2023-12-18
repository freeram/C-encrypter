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

int pad_buffer(uint8_t **buffer, size_t old_len, size_t new_len)
{

    uint8_t *new_buffer = realloc(*buffer, new_len);
    if (!new_buffer)
    {
        return 1;
    }

    // Initialize the new portion of the buffer to 0
    memset(new_buffer + old_len, 0, new_len - old_len);

    *buffer = new_buffer;

    return 0; // Success
}

int main(int argc, char *argv[])
{
    // Getting flags
    int gflag = 0;         // -g: Generate a key flag
    int dflag = 0;         // -d: Decrypt flag
    char *keyvalue = NULL; // -k: Get key from CLI flag

    getflags(argc, argv, &gflag, &dflag, &keyvalue);

    if (gflag && dflag)
    {
        printf("Error: Cannot generate key and decrypt at the same time. Exiting\n");
        return 0;
    }
    if (gflag && (keyvalue != NULL))
    {
        printf("Error: Cannot generate key and parse key at the same time. Exiting\n");
        return 0;
    }

    //    uint8_t *key = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}; // Example key
    //    size_t key_len = sizeof(key) / sizeof(key[0]);

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

    // Blowfish takes keys that are a minimum of 32 bits
    if (key_len < 32)
    {
        size_t old_len = key_len;
        while (key_len < 32)
        {
            key_len++;
        }
        if (pad_buffer(&key, old_len, key_len))
        {
            printf("Error: realloc failure in pad_buffer. Exiting\n");
            exit(1);
        }
    }

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
    printf("\n");

    blowfish_init(key, key_len);

    size_t buffer_size;
    uint8_t *buffer = read_stdin_data(&buffer_size);

    // Blowfish takes 64-bit blocks for input
    // Buffer must be padded until it is a multiple of 64 bits
    if (buffer_size % 8 != 0)
    {
        size_t old_size = buffer_size;
        while (buffer_size % 8 != 0)
        {
            buffer_size++;
        }
        if (pad_buffer(&buffer, old_size, buffer_size))
        {
            printf("Error: realloc failure in pad_buffer. Exiting\n");
            exit(1);
        }
    }

    printf("Buffer size: %ld\n", buffer_size);

    printf("Original buffer:\n");
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    encrypt(buffer, buffer_size);

    printf("Encrypted buffer:\n");
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    decrypt(buffer, buffer_size);

    printf("Decrypted buffer:\n");
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    free(buffer);
    free(key);
    return 0;
}
