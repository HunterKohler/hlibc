/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <hlibc/string.h>

int main()
{
    const char input[] = "Burning 'em, if you ain't quick and nimble\n"
                         "I go crazy when I hear a cymbal";
    const char key[] = "ICE";
    const char expected[] =
        "0B3637272A2B2E63622C2E69692A23693A2A3C6324202D623D63343C2A26226324272"
        "765272A282B2F20430A652E2C652A3124333A653E2B2027630C692B20283165286326"
        "302E27282F";

    char out[256];
    char encoded[256];

    size_t keylen = array_size(key) - 1;
    size_t inputlen = array_size(input) - 1;

    for (int i = 0; i < inputlen; i++) {
        out[i] = input[i] ^ key[i % keylen];
    }

    hex_encode(out, inputlen, encoded);
    assert(!strcmp(encoded, expected));
}
