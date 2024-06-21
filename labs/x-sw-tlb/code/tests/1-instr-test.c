#include "rpi.h"

#include "tests/common.h"

void notmain(void) {
    test_setup();

    // mov r0, #0x24
    // bx lr
    PUT32(0x80000000u, 0xe3a00024u);
    PUT32(0x80000004u, 0xe12fff1eu);

    for (size_t i = 0u; i < 16u; i++)
        GET32(0x90000000u + i * 0x1000000u);

    uint32_t (*f)(void) = (void *)0x80000000u;
    assert(f() == 0x24u);
}
