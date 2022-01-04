#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <hlibc/string.h>

int main()
{
    const char hex[] =
        "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736"
        "f6e6f7573206d757368726f6f6d";

    const char base64[] =
        "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    char hex_out[100] = { 0 };
    char base64_out[100] = { 0 };

    hex_decode(hex, hex_out, 100);
    b64_encode(hex, (array_size(hex) - 1) / 2, base64_out);

    assert(!strcmp(base64_out, base64));

    printf("Success\n");
}
