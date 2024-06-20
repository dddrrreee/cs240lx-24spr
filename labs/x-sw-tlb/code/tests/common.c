#include "rpi.h"
#include "full-except.h"

#include "sw-tlb-handler.h"
#include "sw-tlb-mmu.h"
#include "tests/common.h"

void test_setup(void) {
    const uint32_t STACK_START = STACK_ADDR - 0x1000000u;
    const uint32_t INT_STACK_START = INT_STACK_ADDR - 0x1000000u;
    unsigned sw_tlb_idx = 0u;

    // Everything should be in domain 1
    sw_tlb_mmu_init(0b0100u);

    // Map the code, heap, and stack
    pin_t kern_perm =
        pin_16mb(pin_mk_global(dom_kern, perm_rw_priv, MEM_wb_noalloc));
    sw_tlb_set(sw_tlb_idx++, 0x00000000u, 0x00000000u, kern_perm);
    sw_tlb_set(sw_tlb_idx++, STACK_START, STACK_START, kern_perm);
    sw_tlb_set(sw_tlb_idx++, INT_STACK_START, INT_STACK_START, kern_perm);
    // Map devices
    pin_t dev_perm = pin_16mb(pin_mk_device(dom_kern));
    sw_tlb_set(sw_tlb_idx++, 0x20000000u, 0x20000000u, dev_perm);

    sw_tlb_mmu_enable();

    // The heap can go until the end of the first page, or up to 16MiB
    kmalloc_init_set_start((void *)0x100000u, 0xf00000u);

    // Have the student install their exception handler
    full_except_install(1);
    sw_tlb_handler_initialize(sw_tlb_idx);
}
