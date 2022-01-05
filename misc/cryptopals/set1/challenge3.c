#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <hlibc/string.h>
#include "shared.h"

int main()
{
    const char input[] =
        "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    const char expected[] = "Cooking MC's like a pound of bacon";

    char decoded[100];
    size_t n = strlen(input) / 2;
    uint8_t low_key = 0;
    double low_error = INFINITY;

    hex_decode(input, decoded, array_size(decoded));

    for (int key = 0; key < 256; key++) {
        double error = single_xor_error(decoded, n, key);
        if (error < low_error) {
            low_key = key;
            low_error = error;
        }
    }

    for (int i = 0; i < n; i++)
        decoded[i] ^= low_key;

    decoded[n] = 0;
    assert(!strcmp(expected, decoded));
}
