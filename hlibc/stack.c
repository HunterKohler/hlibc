#include <hlibc/math.h>
#include <hlibc/stack.h>
#include <hlibc/string.h>

int stack_init(struct stack *st, void *data, size_t capacity)
{
    st->data = data;
    st->size = 0;
    st->capacity = capacity;
    return 0;
}

void *stack_alloc(struct stack *st, size_t size)
{
    if (st->size + size > st->capacity)
        return NULL;

    void *ret = (char *)st->data + st->size;
    st->size += size;
    return ret;
}

void *stack_pop(struct stack *st, size_t size)
{
    st->size -= min(st->size, size);
    return (char *)st->data + st->size;
}

void *stack_push(struct stack *st, const void *src, size_t size)
{
    void *ret = stack_alloc(st, size);
    if (!ret)
        return NULL;
    memcpy(ret, src, size);
    return ret;
}

char *stack_strpush(struct stack *st, const char *src)
{
    char *data = st->data;
    char *ret = data + st->size;
    size_t size = st->size;

    while (*src) {
        data[size++] = *src++;

        if (size >= st->capacity)
            return NULL;
    }

    data[size++] = 0;
    st->size = size;
    return ret;
}

char *stack_strnpush(struct stack *st, const char *src, size_t n)
{
    char *data = st->data;
    char *ret = data + st->size;
    size_t size = st->size;

    while (*src && size - st->size <= n) {
        data[size++] = *src++;

        if (size >= st->capacity)
            return NULL;
    }

    data[size++] = 0;
    st->size = size;
    return ret;
}

void *stack_cpy(struct stack *dest, const struct stack *src)
{
    return stack_push(dest, src->data, src->size);
}

int stack_swap(struct stack *a, struct stack *b)
{
    if (a->size > b->capacity || b->size > a->capacity)
        return ENOMEM;

    memswap(a->data, b->data, max(a->size, b->size));

    size_t tmp = a->size;
    a->size = b->size;
    b->size = tmp;
    return 0;
}
