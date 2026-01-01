#define _GNU_SOURCE
#include "vm-circ-buf.h"

#include <assert.h>
#include <sys/mman.h>

bool vm_circ_buf_create(vm_circ_buf_t *buf) {

    if (buf == NULL)
        return false;

    int store_fd = -1;
    void *window = MAP_FAILED;
    int res_ftruncate = -1;
    void *res_mmap0 = MAP_FAILED;
    void *res_mmap1 = MAP_FAILED;

    // Create a page-long file for the backing store. This is done with
    // `memfd_create`.
    store_fd = memfd_create("vm-circ-store", MFD_CLOEXEC);
    if (store_fd == -1)
        goto fail_clean;
    res_ftruncate = ftruncate(store_fd, VM_CIRC_PAGESIZE);
    if (res_ftruncate == -1)
        goto fail_close;

    // Reserve two pages of memory with `mmap` for the window
    window = mmap(NULL, 2ul * VM_CIRC_PAGESIZE, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (window == MAP_FAILED)
        goto fail_close;

    // Map the backing store into the window (twice)
    res_mmap0 = mmap(window, VM_CIRC_PAGESIZE, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_FIXED, store_fd, 0);
    if (res_mmap0 == MAP_FAILED)
        goto fail_unmap;
    res_mmap1 = mmap(window + VM_CIRC_PAGESIZE, VM_CIRC_PAGESIZE,
                    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, store_fd,
                    0);
    if (res_mmap1 == MAP_FAILED)
        goto fail_unmap;
    assert(res_mmap0 == window);
    assert(res_mmap1 == window + VM_CIRC_PAGESIZE);

    // We can close the backing store's file descriptor now. It's mapped into
    // our address space, and it'll automatically be cleaned up when the window
    // is unmapped.
    close(store_fd);

    buf->window = window;
    buf->read_ptr = window;
    buf->write_ptr = window;
    return true;

fail_unmap:
    assert(window != MAP_FAILED);
    assert(window != NULL);
    munmap(window, 2ul * VM_CIRC_PAGESIZE);
fail_close:
    assert(store_fd != -1);
    close(store_fd);
fail_clean:
    return false;
}

void vm_circ_buf_destroy(vm_circ_buf_t *buf) {
    if (buf == NULL)
        return;
    if (buf->window != NULL && buf->window != MAP_FAILED)
        munmap(buf->window, 2ul * VM_CIRC_PAGESIZE);
    buf->window = NULL;
    buf->read_ptr = NULL;
    buf->write_ptr = NULL;
}
