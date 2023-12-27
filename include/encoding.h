#ifndef ENCODING_H
#define ENCODING_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Takes in a buffer of uint8_t and returns a pointer to a dynamically allocated array of base64 chars
// Output length is saved into size_t *output_length
// Returns NULL on error
char *base64_encode(const uint8_t *buffer, size_t buffer_size, size_t *output_length);

// Takes in a buffer of chars and returns a pointer to a dynamically allocated buffer of uint8_t
// Output length is saved into size_t *output_length
// Returns NULL on error
uint8_t *base64_decode(const char *input, size_t *output_length);

#endif
