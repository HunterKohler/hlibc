#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <hlibc/string.h>

int main()
{
    const char hex[] = "1C0111001F010100061A024B53535009181C";
    const char key[] = "686974207468652062756C6C277320657965";
    const char expected[] = "746865206B696420646F6E277420706C6179";

    size_t n = strlen(hex);

    char x[100];
    char y[100];
    char xor_out[100];
    char encoded[100];

    hex_decode(hex, x, 100);
    hex_decode(key, y, 100);

    for (int i = 0; i < n; i++)
        xor_out[i] = x[i] ^ y[i];

    hex_encode(xor_out, n, encoded);
    assert(!strcmp(expected, encoded));

    printf("Success\n");
}
