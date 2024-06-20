#include "vm-circ-buf.h"

vm_circ_buf_t vm_circ_buf_init(vm_pt_t *pt, pin_t attrs, void *store,
                               void *window) {
    uint32_t store_addr = (uint32_t)store;
    uint32_t window_addr = (uint32_t)window;

    // It must be the case that both the store and window are page-aligned,
    // otherwise the scheme won't work
    assert(store_addr % VM_CIRC_PAGESIZE == 0);
    assert(window_addr % VM_CIRC_PAGESIZE == 0);
    // Furthermore, the window should be in high memory, while the store should
    // be in low memory. This is because low memory contains the identity-mapped
    // kernel. (Note that this is an inversion of the usual setup.)
    assert(store_addr < 0x80000000ul);
    assert(window_addr >= 0x80000000ul);

    vm_map_sec(pt, window_addr, store_addr, attrs);
    vm_map_sec(pt, window_addr + VM_CIRC_PAGESIZE, store_addr, attrs);

    return (vm_circ_buf_t){
        .store = store,
        .window = window,
        .read_ptr = window,
        .write_ptr = window,
    };
}
