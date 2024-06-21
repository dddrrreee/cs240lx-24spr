#include "sw-tlb-handler.h"

#include "rpi.h"
#include "asm-helpers.h"
#include "full-except.h"

#include "sw-tlb-mmu.h"
#include "sw-tlb-map.h"

unsigned sw_tlb_num_exceptions = 0;

// Functions to determine what kind of fault was taken. We should always get
// translation section faults.
cp_asm_get(dfsr, p15, 0, c5, c0, 0)
cp_asm_get(ifsr, p15, 0, c5, c0, 1)
cp_asm_get(far, p15, 0, c6, c0, 0)
cp_asm_get(ifar, p15, 0, c6, c0, 2)

// The part of the TLB lockdown registers we're allowed to use
static unsigned lockdown_off = 8;
static unsigned lockdown_len = 0;

// Map for storing virtual to physical mappings
static sw_tlb_map_t map = {
    .entries = NULL,
    .log_capacity = 0,
    .size = 0,
};

// Evict in a round-robin fashion. This counter holds the next index to evict.
static unsigned round_robin_counter = 0;

static void sw_tlb_handler_common(uint32_t faulting_va) {
    assert(lockdown_len >= 2);
    assert(map.entries != NULL);
    assert(map.log_capacity > 0);
    sw_tlb_num_exceptions++;
    faulting_va &= ~0xfffu;

    uint32_t faulting_vpn = faulting_va >> 12;
    uint32_t faulting_pa;
    uint32_t faulting_pfn;

    int have_map = sw_tlb_map_contains(&map, faulting_vpn, &faulting_pfn);
    if (!have_map) {
        faulting_pa = (uint32_t)kmalloc_aligned(4096, 4096);
        faulting_pfn = faulting_pa >> 12;
        sw_tlb_map_add(&map, faulting_vpn, faulting_pfn);
    } else {
        faulting_pa = faulting_pfn << 12;
    }
    assert(faulting_va >> 12 == faulting_vpn);
    assert(faulting_pa >> 12 == faulting_pfn);
    assert((faulting_va & 0xfffu) == 0);
    assert((faulting_pa & 0xfffu) == 0);

    assert(round_robin_counter < lockdown_len);
    sw_tlb_set(
        lockdown_off + round_robin_counter, faulting_va, faulting_pa,
        pin_4k(pin_mk_global(dom_kern, perm_rw_priv, MEM_wb_noalloc)));
    round_robin_counter++;
    if (round_robin_counter >= lockdown_len)
        round_robin_counter = 0;
}

static void sw_tlb_handler_i(regs_t *r) {
    // We should only get translation section faults
    assert((ifsr_get() & 0x40fu) == 0x005u);

    uint32_t addr = ifar_get();
    output("prefetch abort for %x\n", addr);
    sw_tlb_handler_common(addr);
}

static void sw_tlb_handler_d(regs_t *r) {
    // We should only get translation section faults
    assert((dfsr_get() & 0x40fu) == 0x005u);

    uint32_t addr = far_get();
    output("data abort for %x\n", addr);
    sw_tlb_handler_common(addr);
}

void sw_tlb_handler_initialize(unsigned lockdown_start_idx) {

    assert(lockdown_start_idx < 8);
    lockdown_off = lockdown_start_idx;
    lockdown_len = 8 - lockdown_start_idx;

    map = sw_tlb_map_create();

    full_except_set_prefetch(sw_tlb_handler_i);
    full_except_set_data_abort(sw_tlb_handler_d);
}
