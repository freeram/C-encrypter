#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdlib.h>
#include <stdint.h>

// Define any constants here, e.g., key sizes, block sizes
void blowfish_init(uint8_t *key, size_t key_len);

void encrypt(uint8_t *buffer, size_t buffer_size);

void decrypt(uint8_t *buffer, size_t buffer_size);

#endif
