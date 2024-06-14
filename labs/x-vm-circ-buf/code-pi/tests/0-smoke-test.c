#define TEST_NAME "0-smoke"
#include "tests/test-common.c.inc"

void run_test(vm_circ_buf_t buf) {
    // Check that the mappings are correct
    buf.window[0] = 240u;
    assert(buf.store[0] == 240u);
    buf.window[VM_CIRC_PAGESIZE] = 140u;
    assert(buf.store[0] == 140u);
}
