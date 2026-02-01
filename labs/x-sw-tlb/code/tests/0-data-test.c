#include "rpi.h"

#include "tests/common.h"

void notmain(void) {
    test_setup();

    PUT32(0x80000000u, 0xdeadbeefu);
    assert(GET32(0x80000000u) == 0xdeadbeefu);
}
