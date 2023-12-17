#include <stdio.h>
#include "encryption.h"
// #include "filereader.h"

int main(int argc, char *argv[])
{
    uint8_t key[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}; // Example key
    size_t key_len = sizeof(key) / sizeof(key[0]);

    blowfish_init(key, key_len);

    uint8_t buffer[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00, 0x00, 0x00, 0x00};
    size_t buffer_size = sizeof(buffer) / sizeof(buffer[0]);

    printf("Original buffer:\n");
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    encrypt(buffer, buffer_size);

    printf("Encrypted buffer:\n");
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    decrypt(buffer, buffer_size);

    printf("Decrypted buffer:\n");
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    return 0;
}
