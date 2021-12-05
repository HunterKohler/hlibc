#include <stdio.h>
#include <assert.h>
#include <hlibc/murmur_hash.h>

/*
 * Remark: Well constructed test
 */
void test_murmur_hash_x86_32()
{
    struct {
        const char *key;
        size_t len;
        uint32_t seed;
        uint32_t expected;
    } cases[] = {
        {                "",  0,          0,          0},
        {                "",  0,          1, 0x514e28b7},
        {                "",  0, 0xffffffff, 0x81f16f39},
        {"\xff\xff\xff\xff",  4,          0, 0x76293b50},
        {"\x21\x43\x65\x87",  4,          0, 0xf55b516b},
        {"\x21\x43\x65\x87",  4, 0x5082edee, 0x2362f9de},
        {    "\x21\x43\x65",  3,          0, 0x7e4a8634},
        {        "\x21\x43",  2,          0, 0xa0f7b07a},
        {            "\x21",  1,          0, 0x72661cf4},
        {"\x00\x00\x00\x00",  4,          0, 0x2362F9DE},
        {    "\x00\x00\x00",  3,          0, 0x85F0B427},
        {        "\x00\x00",  2,          0, 0x30F4C306},
        {            "\x00",  1,          0, 0x514E28B7},
        {   "Hello, world!", 13,       1234, 0xfaf6cdb3},
        {   "Hello, world!", 13,       4321, 0xbf505788},
    };

    for (int i = 0; i < sizeof(cases) / sizeof(*cases); i++) {
        uint32_t hash =
            murmur_hash_x86_32(cases[i].key, cases[i].len, cases[i].seed);
        assert(hash == cases[i].expected);
    }
}

int main()
{
    test_murmur_hash_x86_32();
    printf("Success\n");
}
