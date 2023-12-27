#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>

// Determining endianness
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define IS_BIG_ENDIAN
#endif

// Initiates blowfish encryption with a key from 32 bits up to 448 bits.
// Cipher mode: ECB.
void blowfish_init(uint8_t *key, size_t key_len);

// Takes in a buffer that is a multiple of 64 bits and encrypts it with Blowfish.
void encrypt(uint8_t *buffer, size_t buffer_size);

// Takes in a buffer that is a multiple of 64 bits and decrypts it with Blowfish.
void decrypt(uint8_t *buffer, size_t buffer_size);

#endif
