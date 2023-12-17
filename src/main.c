#include <stdio.h>
#include "encryption.h"
#include "filereader.h"

int main(int argc, char *argv[])
{
    uint8_t key[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}; // Example key
    size_t key_len = sizeof(key) / sizeof(key[0]);

    blowfish_init(key, key_len);

    size_t buffer_size;
    uint8_t *buffer = read_stdin_data(&buffer_size);

    printf("Original buffer:\n");
    for (size_t i = 0; i < buffer_size-1; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    encrypt(buffer, buffer_size);

    printf("Encrypted buffer:\n");
    for (size_t i = 0; i < buffer_size-1; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    decrypt(buffer, buffer_size);

    printf("Decrypted buffer:\n");
    for (size_t i = 0; i < buffer_size-1; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    return 0;
}
