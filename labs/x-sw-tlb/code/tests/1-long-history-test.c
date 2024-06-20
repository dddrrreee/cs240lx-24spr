#include "rpi.h"

#include "tests/common.h"

void notmain(void) {
    test_setup();

    for (size_t i = 0u; i < 512u; i++) {
        PUT32(0x80000000u + i * 0x1000u, i);
        assert(GET32(0x80000000u + i * 0x1000u) == i);
    }
    for (size_t i = 0u; i < 512u; i++)
        assert(GET32(0x80000000u + i * 0x1000u) == i);
}
