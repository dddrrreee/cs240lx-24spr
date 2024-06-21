#include "rpi.h"

#include "sw-tlb-handler.h"
#include "tests/common.h"

// Generator for normal random numbers. This just uses an LCG.
static uint32_t rand(uint32_t *seed) {
    *seed = *seed * 1664525 + 1013904223;
    return *seed;
}

// Generator for virtual addresses. These have the additional requirement of
// being aligned to 4KiB, being in the range 0x80000000 to 0xfffff000, and never
// being the first page in that range. They also have to never repeat. To do
// this, we use a primitive polynomial in GF(2^19).
static uint32_t next_va(uint32_t va) {
    // Compute the seed value from the address
    assert((va & 0xfff) == 0);
    assert(va >= 0x80000000u);
    uint32_t seed = (va >> 12) - 0x80000u;
    assert(seed != 0);

    // Multiply by x modulo x^19 + x^5 + x^2 + x^1 + 1
    uint32_t next = seed << 1;
    if ((next & ~0x7ffffu) != 0) {
        next &= 0x7ffffu;
        next ^= 0x00023u;
    }

    assert(next != 0);
    return (0x80000u + next) << 12;
}

void notmain(void) {
    test_setup();

    uint32_t seed = 0x24142414u;
    uint32_t seed_va = (rand(&seed) & ~0xfffu) | 0x80000000u;
    assert(seed_va != 0x80000000u);

    uint32_t *result = (uint32_t *)0x80000000;
    uint32_t *a = (uint32_t *)seed_va;
    uint32_t *b = (uint32_t *)next_va(seed_va);

    // Populate the previous pages with random values
    for (size_t i = 0u; i < 1024u; i++) {
        a[i] = rand(&seed);
        b[i] = rand(&seed);
    }

    for (size_t n = 0u; n < 1024u; n++) {
        // Get a new page. It should be all zero
        uint32_t *x = (uint32_t *)next_va((uint32_t)b);
        for (size_t i = 0u; i < 1024u; i++)
            assert(x[i] == 0u);
        // Sum the previous two pages
        for (size_t i = 0u; i < 1024u; i++)
            x[i] = a[i] + b[i];
        // Place into the result buffer
        result[n] = (uint32_t)x[1023u];
        // Update pointers
        a = b;
        b = x;
    }

    // Print a checksum
    uint32_t sum = 0u;
    for (size_t i = 0u; i < 1024u; i++)
        sum += result[i];
    output("Checksum: %x\n", sum);

    // Print stats
    output("Exceptions: %u\n", sw_tlb_num_exceptions);
    output("Memory Use: %u B\n",
        (uint32_t)((char *)kmalloc_heap_ptr() - (char *)kmalloc_heap_start()));
}
