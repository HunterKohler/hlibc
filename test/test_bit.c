#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <hlibc/bit.h>

const char hello_world[] = "Hello world!";
const char hello_world_bin[] =
    "0100100001100101011011000110110001101111001000000111011101101111011100100"
    "11011000110010000100001";

void test_bit_at()
{
    size_t bit_len = strlen(hello_world_bin);

    for (int i = 0; i < bit_len; i++) {
        assert(bit_at(hello_world, i) == (hello_world_bin[i] == '1'));
    }
}

void test_rotl_32()
{
    // Generated rotations
    uint32_t inputs[] = {
        0x33eaeb09, 0x67d5d612, 0xcfabac24, 0x9f575849, 0x3eaeb093, 0x7d5d6126,
        0xfabac24c, 0xf5758499, 0xeaeb0933, 0xd5d61267, 0xabac24cf, 0x5758499f,
        0xaeb0933e, 0x5d61267d, 0xbac24cfa, 0x758499f5, 0xeb0933ea, 0xd61267d5,
        0xac24cfab, 0x58499f57, 0xb0933eae, 0x61267d5d, 0xc24cfaba, 0x8499f575,
        0x0933eaeb, 0x1267d5d6, 0x24cfabac, 0x499f5758, 0x933eaeb0, 0x267d5d61,
        0x4cfabac2, 0x99f57584,
    };

    for (int i = 0; i < 32; i++) {
        assert(rotl_32(inputs[0], i) == inputs[i]);
    }
}

int main()
{
    test_bit_at();
    test_rotl_32();
    printf("Success\n");
}
