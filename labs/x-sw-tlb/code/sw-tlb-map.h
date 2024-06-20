//! \file sw-tlb-hashmap.h
//! \brief Hashmap to store the mappings between virtual and physical addresses
//! \author Ammar Ratnani <ammrat13@gmail.com>
//!
//! We need to have a backing store in main memory to remember all the mappings
//! we've had in the past so that they can safely be evicted from the TLB. This
//! solution implements it as a hashmap, where the key is the virtual page
//! number and the value is the physical frame number.

#pragma once

#include "rpi.h"

_Static_assert(sizeof(unsigned) == sizeof(uint32_t));
_Static_assert(sizeof(size_t) == sizeof(uint32_t));

//! \brief Type of each entry in the hashmap
//!
//! Note that we don't provide a way to delete entries from the hashmap, as
//! evidenced by the fact we don't have a way to mark tombstones. This is fine,
//! because we only serve one process.
typedef struct sw_tlb_map_entry_t {
    int valid: 1;
    uint32_t vpn: 20;
    uint32_t pfn: 20;
} sw_tlb_map_entry_t;

_Static_assert(sizeof(sw_tlb_map_entry_t) == 8);

//! \brief Structure containing metadata for the entries array
typedef struct sw_tlb_map_t {
    sw_tlb_map_entry_t *entries;
    size_t log_capacity;
    size_t size;
} sw_tlb_map_t;

//! \brief Create a hashmap
sw_tlb_map_t sw_tlb_map_create(void);

//! \brief Check if a virtual page is in the hashmap
//! \param[in] map The hashmap to check
//! \param[in] va The virtual page number to check
//! \param[out] pa The physical frame number if it is in the hashmap
//! \return 1 if the virtual page is in the hashmap, 0 otherwise
int sw_tlb_map_contains(const sw_tlb_map_t *map, uint32_t vpn, uint32_t *pfn);

//! \brief Add an entry to a hashmap
//!
//! This function can trigger a resizing of the map, so the `entries` field of
//! the map may change.
//!
//! \param[inout] map The hashmap to add to
//! \param[in] va The virtual page number to add
//! \param[in] pa The physical frame number to add
void sw_tlb_map_add(sw_tlb_map_t *map, uint32_t vpn, uint32_t pfn);
