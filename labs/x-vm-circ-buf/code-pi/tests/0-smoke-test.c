#include "rpi.h"

#include "tests/common.h"
#include "vm-circ-buf.h"

void notmain(void) {
    vm_circ_buf_t buf = test_setup();

    // Check that the mappings are correct
    buf.window[0] = 240u;
    assert(buf.store[0] == 240u);
    buf.window[VM_CIRC_PAGESIZE] = 140u;
    assert(buf.store[0] == 140u);
}
