#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "shared.h"

int main()
{
    // const char input[] =
    //     "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    // size_t n = sizeof(input) - 1;

    // uint8_t low_key;
    // double low_err = INFINITY;

    // for (int i = 0, i < 256; i++) {
    //     int count[256] = { 0 };
    //     double err = 0;

    //     for (int j = 0; j < n; j++)
    //         count[input[j]]++;

    //     for (int j = 0; j < 256; j++)
    //         err += fabs(en_letter_freq[j] - count[j] / n);

    //     if (err < low_err)
    //         low_key = i;
    // }

    // printf("key: %d, %c\n", low_key, low_key);
    // printf("err: %lf\n", low_err);
    // printf("out: %s\n");

    // hex_decode(input, out);

    // byte_t key = single_xor_key(out, n / 2);

    // for (int i = 0; i < n / 2; i++)
    //     out[i] ^= key;

    // assert(!memcmp(out, "Cooking MC's like a pound of bacon", n / 2));

    // printf("Success\n");
}
