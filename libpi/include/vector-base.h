#ifndef __VECTOR_BASE_SET_H__
#define __VECTOR_BASE_SET_H__
#include "libc/bit-support.h"
#include "asm-helpers.h"

/*
 * vector base address register:
 *   3-121 --- let's us control where the exception jump table is!
 *
 * defines:
 *  - vector_base_set
 *  - vector_base_get
 */

// return the current value vector base is set to.
static inline void *vector_base_get(void) {
    void *ret;
    asm volatile ("mrc p15, 0, %0, c12, c0, 0" : "=r" (ret) : : );
    prefetch_flush();
    return ret;
}

// check that not null and alignment is good.
static inline int vector_base_chk(void *vector_base) {
    uint32_t base = (uint32_t)vector_base;
    return base != 0u && (base & 0x1fu) == 0u;
}

// set vector base: must not have been set already.
static inline void vector_base_set(void *vec) {
    if(!vector_base_chk(vec))
        panic("illegal vector base %p\n", vec);

    void *v = vector_base_get();
    // if already set to the same vector, just return.
    if(v == vec)
        return;

    if(v)
        panic("vector base register already set=%p\n", v);

    asm volatile ("mcr p15, 0, %0, c12, c0, 0" : : "r" (vec) :);
    prefetch_flush();

    // make sure it equals <vec>
    v = vector_base_get();
    if(v != vec)
        panic("set vector=%p, but have %p\n", vec, v);
}

// set vector base to <vec> and return old value: could have
// been previously set (i.e., non-null).
static inline void *
vector_base_reset(void *vec) {
    if(!vector_base_chk(vec))
        panic("illegal vector base %p\n", vec);

    void *old_vec = vector_base_get();
    if(vec == old_vec)
        return old_vec;

    asm volatile ("mcr p15, 0, %0, c12, c0, 0" : : "r" (vec) :);
    prefetch_flush();

    assert(vector_base_get() == vec);
    return old_vec;
}
#endif
