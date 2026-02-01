#include "rpi.h"

#include "tests/common.h"

void notmain(void) {
    test_setup();

    for (size_t i = 0; i < 4u * 1024u * 1024u; i += 4u)
        assert(GET32(0x80000000u + i) == 0u);
}
