#include <hlibc/def.h>
#include <hlibc/string.h>
#include <hlibc/math.h>

/*
 * Object agnostic stack.
 */
struct stack {
    size_t size;
    size_t capacity;
    void *data;
};

/*
 * Initialize stack statically.
 */
#define STACK_INIT(init_data, init_capacity) \
    ((struct stack){ .data = init_data, .capacity = init_capacity, .size = 0 })

/*
 * Initialize stack dynamically.
 */
int stack_init(struct stack *st, void *data, size_t capacity);

/*
 * Allocate a region on the top of the stack.
 */
void *stack_alloc(struct stack *st, size_t size);

/*
 * Remove region from top of the stack.
 */
void *stack_pop(struct stack *st, size_t size);

/*
 * Allocate and copy data to top of the stack.
 */
void *stack_push(struct stack *st, const void *src, size_t size);

/*
 * Allocate space for, and copy a string onto the top of the stack.
 */
char *stack_strpush(struct stack *st, const char *src);

/*
 * Allocate space for, and copy a string of at most `n` characters onto the top
 * of the stack. Will null terminate destination string for a `i + 1`th
 * character for the appropriate `1 <= i <= n`.
 */
char *stack_strnpush(struct stack *st, const char *src, size_t n);

/*
 * Copy the contents of a stack onto the top of another.
 */
void *stack_cpy(struct stack *dest, const struct stack *src);

/*
 * Swap the contents of two stacks.
 */
int stack_swap(struct stack *a, struct stack *b);
