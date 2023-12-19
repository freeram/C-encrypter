#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdlib.h>
#include <stdint.h>
#include <byteswap.h>

// Determining endianness
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define IS_BIG_ENDIAN 1
#else
#define IS_BIG_ENDIAN 0
#endif

void blowfish_init(uint8_t *key, size_t key_len);

void encrypt(uint8_t *buffer, size_t buffer_size);

void decrypt(uint8_t *buffer, size_t buffer_size);

#endif
