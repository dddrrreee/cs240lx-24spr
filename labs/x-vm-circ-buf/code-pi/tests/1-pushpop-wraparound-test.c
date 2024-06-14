#define TEST_NAME "1-pushpop-wraparound"
#include "tests/test-common.c.inc"

static uint8_t skew_buf[0x100] = {0};

void run_test(vm_circ_buf_t buf) {

    // Skew the buffer by 0x100 bytes
    vm_circ_buf_push(&buf, skew_buf, 0x100);
    assert(vm_circ_buf_get_size(&buf) == 0x100);
    vm_circ_buf_pop(&buf, 0x100);
    assert(vm_circ_buf_get_size(&buf) == 0);

    // Push an entire page's worth of data
    uint8_t test_data = 240u;
    for (size_t i = 0; i < VM_CIRC_PAGESIZE; i++)
        vm_circ_buf_push(&buf, &test_data, 1);
    assert(vm_circ_buf_get_size(&buf) == VM_CIRC_PAGESIZE);

    // Peek at the buffer. It should be the same as the test data.
    uint8_t *peek_data = vm_circ_buf_peek(&buf);
    for (size_t i = 0; i < VM_CIRC_PAGESIZE; i++)
        assert(peek_data[i] == test_data);

    // Pop all the data
    vm_circ_buf_pop(&buf, VM_CIRC_PAGESIZE);
    assert(vm_circ_buf_get_size(&buf) == 0);

    // Now do it again
    for (size_t i = 0; i < VM_CIRC_PAGESIZE; i++)
        vm_circ_buf_push(&buf, &test_data, 1);
    assert(vm_circ_buf_get_size(&buf) == VM_CIRC_PAGESIZE);
    peek_data = vm_circ_buf_peek(&buf);
    for (size_t i = 0; i < VM_CIRC_PAGESIZE; i++)
        assert(peek_data[i] == test_data);
}
