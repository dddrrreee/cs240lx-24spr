//! \file vm-circ-buf.h
//! \brief Virtual memory circular buffer implementation
//! \author Ammar Ratnani <ammrat13@gmail.com>
//!
//! This implementation of a circular buffer has two contiguous virtual pages
//! pointing to the same physical frame of data. This way, all data in the
//! circular buffer can be read and written contiguously. And it's more
//! efficient to boot.
//!
//! Throughout this file, we use the terms "store" and "window". The store is
//! the physical frame that holds the data, while the window is the two virtual
//! pages that map to the store.
//!
//! For simplicity, we assume that exactly one physical frame and exactly two
//! virtual pages are used. There are more complicated schemes, from using
//! multiple physical frames, to "leapfrogging" virtual pages. But, we don't do
//! this. As a result, the circular buffer can hold at most one page of data.

#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

_Static_assert(sizeof(uint8_t) == sizeof(unsigned char));
_Static_assert(sizeof(void *) == sizeof(unsigned long));
_Static_assert(sizeof(size_t) == sizeof(unsigned long));

//! \brief Size of a circular store
//!
//! For this scheme to work, the size of the backing store must be a multiple of
//! the page size. The page size can be determined dynamically on Linux systems
//! using `sysconf`. This macro just aliases that command, and as a result it
//! cannot be used in a constant expression.
//!
//! \see vm_circ_store_t
#define VM_CIRC_PAGESIZE ((size_t)sysconf(_SC_PAGESIZE))

//! \brief The circular buffer "class"
//!
//! This structure holds all the data needed to work with a circular buffer,
//! such as the read and write pointers and the base address.
//!
//! On the Raspberry Pi, this class had a `store` pointer, which pointed to the
//! physical frame of data. On Linux, we only ever get a virtual window, with
//! the physical frame being managed by the kernel. Thus, we don't need the
//! `store` pointer.
typedef struct vm_circ_buf_t {
    //! \brief Virtual address of the window into the backing store
    uint8_t *window;

    uint8_t *read_ptr;
    uint8_t *write_ptr;
} vm_circ_buf_t;

//! \brief Create a new circular buffer
//!
//! This function allocates the backing store for the circular buffer, and sets
//! up the window. It returns a circular buffer structure that can be used in
//! other functions. This means the `window` pointer should be set, and the read
//! and write pointers should be set to the beginning of the window.
//!
//! \param[out] buf The space where put the new circular buffer's parameters
//! \return Whether creation succeeded
bool vm_circ_buf_create(vm_circ_buf_t *buf);
//! \brief Destroy an existing circular buffer]
//!
//! This function essentially does the opposite of `vm_circ_buf_create`. It
//! frees the backing store of the circular buffer, meaning the buffer can't be
//! used after this function is called. For safety, this function should set all
//! of the fields of the circular buffer to `NULL`.
//!
//! \param[inout] buf The circular buffer to destroy
void vm_circ_buf_destroy(vm_circ_buf_t *buf);
//! \brief Create a new circular buffer, asserting that it succeeded
//! \see vm_circ_buf_create
static inline vm_circ_buf_t vm_circ_buf_create_assert(void) {
    vm_circ_buf_t buf;
    bool res = vm_circ_buf_create(&buf);
    assert(res);
    return buf;
}

//! \brief Returns the number of bytes currently buffered
//! \param[in] buf The circular buffer to get the size of
size_t vm_circ_buf_get_size(const vm_circ_buf_t *buf);
//! \brief Returns the number of bytes that can be pushed into the buffer
//! \param[in] buf The circular buffer to get the remaining bytes in
size_t vm_circ_buf_get_remaining(const vm_circ_buf_t *buf);

//! \brief Push some data into the buffer
//!
//! If pushing all of the data would overflow the buffer, the function should
//! push only the first part of the data, up until the buffer is full.
//!
//! \param[inout] buf The circular buffer to push data into
//! \param[in] data A pointer to data to push
//! \param[in] len How many bytes to push from the data
void vm_circ_buf_push(vm_circ_buf_t *buf, uint8_t *data, size_t len);
//! \brief Pop some data from the buffer
//!
//! If the number of bytes to pop is greater than the number of bytes in the
//! buffer, the function should just pop all of the data in the buffer.
//!
//! \param[inout] buf The circular buffer to pop data from
//! \param[in] len How many bytes to pop
void vm_circ_buf_pop(vm_circ_buf_t *buf, size_t len);

//! \brief Get a pointer to all the data in the buffer
//!
//! The pointer returned should have as many valid bytes as the buffer has -
//! i.e., the pointer should point to the buffered data. It remains valid until
//! the next pop.
//!
//! \param[in] buf The circular buffer to peek into
uint8_t *vm_circ_buf_peek(const vm_circ_buf_t *buf);
