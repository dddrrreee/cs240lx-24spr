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
#include <stdint.h>
#include "rpi.h"
#include "pt-vm.h"

_Static_assert(sizeof(uint8_t) == sizeof(unsigned char));
_Static_assert(sizeof(void *) == sizeof(unsigned long));
_Static_assert(sizeof(void *) == sizeof(uint32_t));

//! \brief Size of a circular store
//!
//! For this scheme to work, the size of the backing store must be a multiple of
//! the page size. We assume that pages are 1MiB since that's what was covered
//! in class.
//!
//! \see vm_circ_store_t
#define VM_CIRC_PAGESIZE (0x100000ul)

//! \brief The circular buffer "class"
//!
//! This structure holds all the data needed to work with a circular buffer,
//! such as the read and write pointers and the base address.
typedef struct vm_circ_buf_t {
    //! \brief Physical address of the backing store
    uint8_t *store;
    //! \brief Virtual address of the window into the backing store
    uint8_t *window;

    uint8_t *read_ptr;
    uint8_t *write_ptr;
} vm_circ_buf_t;

//! \brief Perform setup for a circular buffer
//!
//! This function computes the mappings that are needed for this circular
//! buffer, and modifies the `vm_pt_t` to include them. The `store` parameter is
//! the intended physical address to map to, and the `window` parameter is the
//! intended virtual address.
//!
//! Note that the `window` pointer is not valid yet. It's just used to set up
//! the `pt` parameter to map the window once the MMU is turned on.
//!
//! This function should return a circular buffer structure that can be used in
//! other functions. This means the `store` and `window` pointers should be set,
//! and the read and write pointers should be set to the beginning of the
//! window.
//!
//! \param[out] pt The pagetable to add the VM mappings to
//! \param[in] attrs The attributes of the mapping
//! \param[in] store The physical address to map to
//! \param[in] window The virtual address to map
//! \return The circular buffer structure other functions can use
vm_circ_buf_t vm_circ_buf_init(vm_pt_t *pr, pin_t attrs, void *store,
                               void *window);

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
