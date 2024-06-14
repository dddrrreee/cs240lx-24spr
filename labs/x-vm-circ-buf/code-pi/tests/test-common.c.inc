//! \file test-common.c.inc
//! \brief Common test setup for the VM circular buffer
//!
//! To use this file, include it in your test file, and define the `run_test`
//! function. You can also define `TEST_NAME` before including this file as a
//! string to be printed.

#include "rpi.h"
#include "pt-vm.h"

#include "vm-circ-buf.h"

// The domain we run kernel code in. For safety, we don't use 0. So, we'll start
// at 1 instead.
static const unsigned DOM_KERNEL = 1;

// Function to actually run the test
void run_test(vm_circ_buf_t buf);

void notmain(void) {

    // We're forced to use the heap because of `procmap`, so we'll put it
    // between 1MiB and 3MiB. The reason it's so large is because we allocate
    // the store from here. We can't statically allocate it for some reason.
    kmalloc_init_set_start((void *)0x100000ul, 0x200000u);

    // Set up the process map. It only allows identity-mapped sections, so we do
    // some mapping now, then use the returned `pt` to map the circular buffer.
    // The default map doesn't map the second part of the heap, so do that.
    procmap_t pm = procmap_default_mk(DOM_KERNEL);
    procmap_push(&pm, pr_ent_mk(0x200000, 0x100000, MEM_RW, DOM_KERNEL));
    vm_pt_t *pt = vm_map_kernel(&pm, 0);

    // Set up the circular buffer
    pin_t buf_attrs = pin_mk_global(DOM_KERNEL, perm_rw_user, MEM_uncached);
    uint8_t *buf_store = kmalloc_aligned(VM_CIRC_PAGESIZE, VM_CIRC_PAGESIZE);
    uint8_t *buf_window = (void *)0x80000000ul;
    vm_circ_buf_t buf = vm_circ_buf_init(pt, buf_attrs, buf_store, buf_window);

    // Turn on the MMU now that all the mappings are set up. The `vm_map_kernel`
    // function implicitly sets the page table base address.
    vm_mmu_enable();
    assert(mmu_is_enabled());

#ifdef TEST_NAME
    output("Running Test: " TEST_NAME "\n");
#else
    output("Running Test\n");
#endif

    run_test(buf);
}
