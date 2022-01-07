#include "testlib.h"

#if __has_attribute(weak)

__attribute__((weak))
int main(int argc, char **argv)
{
    return testlib_main(argc, argv);
}

#endif

