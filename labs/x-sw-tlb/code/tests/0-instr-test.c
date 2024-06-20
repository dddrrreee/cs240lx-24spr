#include "rpi.h"

#include "tests/common.h"

void notmain(void) {
    test_setup();

    PUT32(0x80000000u, 0xe3a00024u);
    PUT32(0x80000004u, 0xe12fff1eu);

    uint32_t (*f)(void) = (void *)0x80000000u;
    assert(f() == 0x24u);
}
