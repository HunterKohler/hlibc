#ifndef HLIBC_STRING_H_
#define HLIBC_STRING_H_

#include <string.h>
#include <hlibc/def.h>

#ifdef __cplusplus
extern "C" {
#endif

struct strbuilder {
    char *buf;
    size_t buf_size;
    size_t str_size;
};

struct strbuilder *strbuilder_new();
struct strbuilder *strbuilder_copy(struct strbuilder *builder);
char *strbuilder_end(struct strbuilder *builder);
bool strbuilder_init(struct strbuilder *builder);
bool strbuilder_append(struct strbuilder *builder, const char *str);

#ifdef __cplusplus
}
#endif

#endif