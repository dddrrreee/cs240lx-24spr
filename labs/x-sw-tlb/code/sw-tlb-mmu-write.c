#include "rpi.h"
#include "asm-helpers.h"

#include "pinned-vm.h"
#include "sw-tlb-mmu.h"

cp_asm(lockdown_index, p15, 5, c15, c4, 2)
cp_asm(lockdown_va, p15, 5, c15, c5, 2)
cp_asm(lockdown_pa, p15, 5, c15, c6, 2)
cp_asm(lockdown_attr, p15, 5, c15, c7, 2)

void sw_tlb_set(unsigned idx,
                uint32_t va,
                uint32_t pa,
                pin_t e) {

    // We only have 8 lockdown entries
    assert(idx < 8);
    // Pages can only go down to 4KiB
    assert((va & 0xfffu) == 0);
    assert((pa & 0xfffu) == 0);

    uint32_t attr = (e.dom << 7) | (e.mem_attr << 1);
    uint32_t va_ent = va | (e.G << 9) | e.asid;
    uint32_t pa_ent = pa | (e.pagesize << 6) | (e.AP_perm << 1) | 1;
    lockdown_index_set(idx);
    lockdown_attr_set(attr);
    lockdown_va_set(va_ent);
    lockdown_pa_set(pa_ent);

    uint32_t x;
    if((x = lockdown_va_get()) != va_ent)
        panic("lockdown va: expected %x, have %x\n", va_ent,x);
    if((x = lockdown_pa_get()) != pa_ent)
        panic("lockdown pa: expected %x, have %x\n", pa_ent,x);
    if((x = lockdown_attr_get()) != attr)
        panic("lockdown attr: expected %x, have %x\n", attr,x);
}
