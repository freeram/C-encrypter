#include <ctype.h>
#include <unistd.h>
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

int main(int argc, char *argv[])
{
    // Getting flags
    int gflag = 0;
    int dflag = 0;
    char *keyvalue = NULL;

    getflags(argc, argv, &gflag, &dflag, &keyvalue);

    printf("gflag = %d, dflag = %d, keyvalue = %s\n\n",
           gflag, dflag, keyvalue);
    //

    uint8_t key[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}; // Example key
    size_t key_len = sizeof(key) / sizeof(key[0]);

    blowfish_init(key, key_len);

    size_t buffer_size;
    uint8_t *buffer = read_stdin_data(&buffer_size);

    printf("Original buffer:\n");
    for (size_t i = 0; i < buffer_size - 1; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");

    encrypt(buffer, buffer_size);

    printf("Encrypted buffer:\n");
    for (size_t i = 0; i < buffer_size - 1; i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    decrypt(buffer, buffer_size);

    printf("Decrypted buffer:\n");
    for (size_t i = 0; i < buffer_size - 1; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");

    free(buffer);
    return 0;
}
