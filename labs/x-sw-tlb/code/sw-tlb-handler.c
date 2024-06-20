#include "sw-tlb-handler.h"

#include "rpi.h"
#include "full-except.h"

#include "sw-tlb-mmu.h"

volatile unsigned sw_tlb_num_exceptions = 0;

// The part of the TLB lockdown registers we're allowed to use
static volatile unsigned lockdown_off = 8;
static volatile unsigned lockdown_len = 0;

static void sw_tlb_handler_i(regs_t *r) {
    panic("Prefetch abort at %x\n", r->regs[REGS_PC]);
}

static void sw_tlb_handler_d(regs_t *r) {
    panic("Data abort at %x\n", r->regs[REGS_PC]);
}

void sw_tlb_handler_initialize(unsigned lockdown_start_idx) {

    assert(lockdown_start_idx < 8);
    lockdown_off = lockdown_start_idx;
    lockdown_len = 8 - lockdown_start_idx;

    full_except_set_prefetch(sw_tlb_handler_i);
    full_except_set_data_abort(sw_tlb_handler_d);
}
