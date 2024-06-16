#include "vm-circ-buf.h"

//! \brief Check that the circular buffer is in a valid state
//! \param[in] buf The circular buffer to check
static void vm_circ_buf_assert(const vm_circ_buf_t *buf) {
    // Both pointers should be in the window
    assert(buf->read_ptr >= buf->window);
    assert(buf->write_ptr >= buf->window);
    assert(buf->read_ptr < buf->window + 2ul * VM_CIRC_PAGESIZE);
    assert(buf->write_ptr < buf->window + 2ul * VM_CIRC_PAGESIZE);
    // The read pointer should be to the left of the write pointer
    assert(buf->read_ptr <= buf->write_ptr);
}

//! \brief Helper function for normalization and validation
//!
//! This function should be called at the end of every function, that modifies
//! the read and write pointers. It ensures that the read pointer is always in
//! the lower half of the window. It also calls `vm_circ_buf_assert`.
//!
//! \see vm_circ_buf_assert
//! \param[inout] buf The circular buffer to renormalize
static void vm_circ_buf_renomalize(vm_circ_buf_t *buf) {
    vm_circ_buf_assert(buf);
    // If the operation we just did caused the read pointer to be in the second
    // page, we need to move it back to the first page, and adjust the write
    // pointer accordingly.
    if (buf->read_ptr >= buf->window + VM_CIRC_PAGESIZE) {
        buf->read_ptr -= VM_CIRC_PAGESIZE;
        buf->write_ptr -= VM_CIRC_PAGESIZE;
    }
}

size_t vm_circ_buf_get_size(const vm_circ_buf_t *buf) {
    vm_circ_buf_assert(buf);
    return buf->write_ptr - buf->read_ptr;
}

size_t vm_circ_buf_get_remaining(const vm_circ_buf_t *buf) {
    vm_circ_buf_assert(buf);
    return VM_CIRC_PAGESIZE - vm_circ_buf_get_size(buf);
}

void vm_circ_buf_push(vm_circ_buf_t *buf, uint8_t *data, size_t len) {
    size_t rem = vm_circ_buf_get_remaining(buf);
    size_t to_push = len > rem ? rem : len;

    memcpy(buf->write_ptr, data, to_push);
    buf->write_ptr += to_push;
    vm_circ_buf_renomalize(buf);
}

void vm_circ_buf_pop(vm_circ_buf_t *buf, size_t len) {
    size_t size = vm_circ_buf_get_size(buf);
    size_t to_pop = len > size ? size : len;

    buf->read_ptr += to_pop;
    vm_circ_buf_renomalize(buf);
}

uint8_t *vm_circ_buf_peek(const vm_circ_buf_t *buf) {
    vm_circ_buf_assert(buf);
    return buf->read_ptr;
}
