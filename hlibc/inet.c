/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <ctype.h>
#include <hlibc/string.h>
#include <hlibc/inet.h>

void ipv4_to_str(const void *restrict src, char *restrict dest)
{
    for (int i = 0; i < 4; i++) {
        int val = ((const uint8_t *)src)[i];

        if (val < 10) {
            dest[0] = '0' + val;
            dest += 1;
        } else if (val < 100) {
            dest[0] = '0' + (val / 10);
            dest[1] = '0' + (val % 10);
            dest += 2;
        } else {
            dest[0] = '0' + (val / 100);
            dest[1] = '0' + ((val / 10) % 10);
            dest[2] = '0' + (val % 10);
            dest += 3;
        }

        *dest++ = '.';
    }

    dest[-1] = 0;
}

int ipv4_to_bin(const char *restrict src, void *restrict dest)
{
    const char *pos = src;
    uint8_t *bin = dest;

    for (int i = 0; i < 4; i++) {
        int val = 0;

        if (isdigit(*pos)) {
            if (*pos == '0') {
                pos++;
            } else {
                val = 10 * val + *pos++ - '0';
                if (isdigit(*pos)) {
                    val = 10 * val + *pos++ - '0';
                    if (isdigit(*pos)) {
                        val = 10 * val + *pos++ - '0';
                        if (val > 255)
                            return EINVAL;
                    }
                }
            }
        } else {
            return EINVAL;
        }

        if (i < 3 && *pos++ != '.')
            return EINVAL;

        bin[i] = val;
    }

    return *pos ? EINVAL : 0;
}

static inline void ipv6_to_str_part(const uint8_t *restrict bin,
                                    char *restrict *destptr)
{
    static const char hex_cset[] = "0123456789abcdef";
    char *dest = *destptr;
    uint8_t vals[] = {
        hex_cset[bin[0] >> 4],
        hex_cset[bin[0] & 15],
        hex_cset[bin[1] >> 4],
        hex_cset[bin[1] & 15],
    };

    int i = 0;
    while (i < 3 && !vals[i])
        i++;
    while (i < 4)
        *dest++ = hex_cset[vals[i]];
    *destptr = dest;
}

void ipv6_to_str(const void *restrict src, char *restrict dest)
{
    const uint8_t *bin = src;
    int max_start = 0;
    int max_size = 0;

    for (int i = 0, start = 0; i <= 16; i += 2) {
        if (i == 16 || (bin[i] && bin[i + 1])) {
            int size = i - start;
            if (size > max_size) {
                max_start = start;
                max_size = size;
            }

            start = i;
        }
    }

    for (int i = 0; i < max_start; i += 2) {
        ipv6_to_str_part(bin, &dest);
        *dest++ = ':';
        bin += 2;
    }

    if (max_size) {
        bin += max_size;
        *dest++ = ':';
    }

    for (int i = max_start + max_size; i < 16; i += 2) {
        ipv6_to_str_part(bin, &dest);
        *dest++ = ':';
        bin += 2;
    }

    *dest = 0;
}

int ipv6_norm(const char *restrict src, char *restrict dest)
{
    uint8_t bin[16];

    if (ipv6_to_bin(src, bin))
        return EINVAL;

    ipv6_to_str(bin, dest);
    return 0;
}

int ipv6_to_bin(const char *restrict src, void *restrict dest)
{
    const char *pos = src;
    uint8_t *bin = dest;
    int comp = -1;
    int count = 0;
    bool mapped = false;

    if (pos[0] == ':' && pos[1] != ':')
        return EINVAL;

    while (*pos && (count + 2 * (comp > -1) < 16)) {
        if (*pos == ':')
            pos++;

        if (*pos == ':') {
            if (comp > -1)
                return EINVAL;
            pos++;
            comp = count;
            if (!*pos)
                break;
        }

        if (!isxdigit(*pos))
            return EINVAL;

        int v = hex_val(*pos++);
        for (int i = 1; i < 4; i++) {
            if (isxdigit(*pos)) {
                v = 16 * v + hex_val(*pos++);
            } else {
                mapped = *pos == '.';
                pos -= i * mapped;
                break;
            }
        }

        if (mapped) {
            if (count < 2 || count > 12 || bin[count - 1] != 0xFF ||
                bin[count - 2] != 0xFF || comp == count)
                return EINVAL;

            for (int i = 0; i < count - 2; i++) {
                if (bin[i])
                    return EINVAL;
            }

            if (ipv4_to_bin(pos, bin + count))
                return EINVAL;

            count += 4;
            break;
        } else {
            bin[count++] = (v >> 4) & 0xFF;
            bin[count++] = v & 0xFF;
        }
    }

    if (!mapped && *pos) {
        return EINVAL;
    } else if (comp > -1) {
        if (count >= 16)
            return EINVAL;

        int len = 16 - count;
        uint8_t *start = bin + comp;

        memmove(start + len, start, len);
        memzero(start, len);
    } else if (count < 16) {
        return EINVAL;
    }

    return 0;
}
