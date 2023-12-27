#ifndef PADDING_H
#define PADDING_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Pads buffer with pkcs5.
// Returns 0 on success, 1 on error.
int pad_buffer_pkcs5(uint8_t **buffer, size_t old_len, size_t new_len);

// Depads buffer padded with pkcs5.
// Returns 0 on success, 1 on error.
int depad_buffer_pkcs5(uint8_t **buffer, size_t *buffer_size);

#endif
