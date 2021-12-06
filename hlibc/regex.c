#include <hlibc/regex.h>

size_t regmatch_len(const regmatch_t *match)
{
    return match->rm_eo - match->rm_so;
}
