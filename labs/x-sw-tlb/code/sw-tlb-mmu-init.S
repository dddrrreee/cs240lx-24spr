#include "rpi-asm.h"
#include "armv6-coprocessor-asm.h"

MK_FN(sw_tlb_mmu_init)

    // Invalidate all of the caches, especially since they might be holding
    // invalid data because of initialization.
    INV_ALL_CACHES(r1)
    INV_TLB(r1)
    // Will deal with BTB later

    // Have to DSB since we did a cache maintenance operation
    DSB(r1)
    // Will PrefetchFlush later

    // Disable hardware page table walks
    mov r1, #0x30
    TTBR_BASE_CTRL_WR(r1)
    // Set the domains to what was passed as a parameter
    DOMAIN_CTRL_WR(r0)

    // Must flush BTB since we wrote to a TTB register
    FLUSH_BTB(r1)

    // Have to prefetch flush because we modified CP15 registers
    PREFETCH_FLUSH(r1)
    bx lr

MK_FN(sw_tlb_mmu_enable)

    // Note that all the caches are already clean

    // Enable the caches, the branch predictor, and the MMU
    CONTROL_REG1_RD(r0)
    orr r0, #(0b11 << 11)
    orr r0, #(0b101 << 0)
    CONTROL_REG1_WR(r0)

    // Must flush the BTB since we enabled the MMU
    FLUSH_BTB(r0)

    // Have to prefetch flush since we modified CP15 registers
    PREFETCH_FLUSH(r0)
    bx lr
