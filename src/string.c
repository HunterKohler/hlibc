#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <hlibc/string.h>

struct strbuilder *strbuilder_new()
{
    struct strbuilder *builder = malloc(sizeof(*builder));

    if (!builder || strbuilder_init(builder)) {
        free(builder);
        return NULL;
    }

    return builder;
}

bool strbuilder_init(struct strbuilder *builder)
{
    builder->buf_size = 100;
    builder->str_size = 0;
    builder->buf = malloc(builder->buf_size);

    if (!builder->buf) {
        return true;
    }

    return false;
}

bool strbuilder_append(struct strbuilder *builder, const char *str)
{
    for (const char *s = str; *s != '\0'; s++) {
        if (builder->str_size == builder->buf_size - 1) {
            builder->buf_size = 3 * builder->buf_size >> 1;
            char *temp = realloc(builder->buf, builder->buf_size);

            if (!temp) {
                return true;
            }

            builder->buf = temp;
        }

        builder->buf[builder->str_size++] = *s;
    }

    return false;
}

struct strbuilder *strbuilder_copy(struct strbuilder *builder)
{
    struct strbuilder *copy = malloc(sizeof(*copy));

    if (!copy) {
        return NULL;
    }

    copy->buf_size = builder->buf_size;
    copy->str_size = builder->str_size;
    copy->buf = malloc(copy->buf_size);

    if (!copy->buf || !memcpy(copy->buf, builder->buf, builder->str_size)) {
        free(copy);
        return NULL;
    }

    return copy;
}

char *strbuilder_end(struct strbuilder *builder)
{
    char *str = realloc(builder->buf, builder->str_size + 1);

    if (!str) {
        return NULL;
    }

    str[builder->str_size] = '\0';

    builder->buf = NULL;
    builder->buf_size = 0;
    builder->buf_size = 0;

    return str;
}