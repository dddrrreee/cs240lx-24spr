//! \file sw-tlb-mmu.h
//! \brief MMU configuration for the software TLB
//! \author Ammar Ratnani <ammrat13@gmail.com>
//!
//! It turns out that configuring the MMU to support the software TLB is
//! sufficiently different from the normal MMU configuration to warrant separate
//! code. Thus, this file allows the user to initialize and enable the MMU so
//! that it doesn't do any hardware page-table walks. Finally, it enables caches
//! and branch prediction to speed up the benchmark.

#pragma once
#include <stdint.h>
#include "pinned-vm.h"

void sw_tlb_mmu_init(uint32_t domain_access_control);
void sw_tlb_mmu_enable(void);

void sw_tlb_set(unsigned idx,
                uint32_t va,
                uint32_t pa,
                pin_t e);
