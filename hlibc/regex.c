#include <hlibc/regex.h>

int regcomp_once(struct regcomp_once *once, regex_t *re, const char *pattern,
                 int cflags)
{
    int ret = 0;
    if (!once->done) {
        spinlock_lock(&once->lock);
        if (!once->done)
            ret = regcomp(re, pattern, cflags);
        spinlock_unlock(&once->lock);
    }
    return ret;
}