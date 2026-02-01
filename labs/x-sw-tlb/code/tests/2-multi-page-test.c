#include "rpi.h"

#include "tests/common.h"

void notmain(void) {
    test_setup();

    // mov r0, #0x90000000
    // ldr r0, [r0]
    // bx lr
    PUT32(0x80000000u, 0xe3a00209u);
    PUT32(0x80000004u, 0xe5900000u);
    PUT32(0x80000008u, 0xe12fff1eu);

    PUT32(0x90000000u, 0xdeadbeefu);

    for (size_t i = 2u; i < 16u; i++)
        GET32(0xa0000000u + i * 0x1000000u);

    uint32_t (*f)(void) = (void *)0x80000000u;
    assert(f() == 0xdeadbeefu);
}
