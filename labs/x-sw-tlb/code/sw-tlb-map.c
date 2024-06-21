#include "sw-tlb-map.h"

//! \brief Get the capacity of the hashmap
static size_t CAP(const sw_tlb_map_t *map) {
    assert(map != NULL);
    assert(map->log_capacity < 30);
    return 1u << map->log_capacity;
}
//! \brief Get the mask for iterating over the hashmap
static size_t MASK(const sw_tlb_map_t *map) {
    return CAP(map) - 1;
}

//! \brief Hash a virtual page number into the hashmap
//! \see https://stackoverflow.com/questions/11871245/knuth-multiplicative-hash
static size_t sw_tlb_map_hash(const sw_tlb_map_t *map, uint32_t vpn) {
    assert(map != NULL);
    assert(vpn < (1u << 20));
    return (2654435761u * vpn) >> (32 - map->log_capacity);
}

sw_tlb_map_t sw_tlb_map_create(void) {

    const size_t INIT_LOG_CAP = 7;
    const size_t INIT_CAP = 1u << INIT_LOG_CAP;

    // The entries are all cleared to zero by `kmalloc`, so they're all invalid
    sw_tlb_map_entry_t *ents = kmalloc(INIT_CAP * sizeof(sw_tlb_map_entry_t));
    assert(ents != NULL);

    return (sw_tlb_map_t) {
        .entries = ents,
        .log_capacity = INIT_LOG_CAP,
        .size = 0,
    };
}

int sw_tlb_map_contains(const sw_tlb_map_t *map, uint32_t vpn, uint32_t *pfn) {
    assert(map != NULL);
    assert(vpn < (1u << 20));

    // Compute the initial index
    size_t initial_idx = sw_tlb_map_hash(map, vpn);
    // Use linear probing
    for (size_t i = 0u; i < CAP(map); i++) {
        // Retrieve
        size_t idx = (initial_idx + i) & MASK(map);
        sw_tlb_map_entry_t *ent = &map->entries[idx];
        // Process
        if (!ent->valid)
            return 0;
        if (ent->vpn == vpn) {
            if (pfn != NULL)
                *pfn = ent->pfn;
            return 1;
        }
    }
    return 0;
}

void sw_tlb_map_add(sw_tlb_map_t *map, uint32_t vpn, uint32_t pfn) {
    assert(map != NULL);
    assert(vpn < (1u << 20));
    assert(pfn < (1u << 20));

    // Resize if necessary
    static int resizing = 0;
    if (map->size >= (CAP(map) / 4) * 3) {
        assert(!resizing);
        resizing = 1;

        // Create a new map with double the capacity
        size_t new_log_cap = map->log_capacity + 1;
        size_t new_cap = 1u << new_log_cap;
        sw_tlb_map_entry_t *new_ents =
            kmalloc(new_cap * sizeof(sw_tlb_map_entry_t));
        assert(new_ents != NULL);
        sw_tlb_map_t new_map = {
            .entries = new_ents,
            .log_capacity = new_log_cap,
            .size = 0,
        };

        // Rehash
        for (size_t i = 0u; i < CAP(map); i++) {
            sw_tlb_map_entry_t *ent = &map->entries[i];
            if (ent->valid)
                sw_tlb_map_add(&new_map, ent->vpn, ent->pfn);
        }
        // Update
        *map = new_map;
        resizing = 0;
    }

    // Insert using linear probing
    size_t initial_idx = sw_tlb_map_hash(map, vpn);
    for (size_t i = 0u; i < CAP(map); i++) {
        size_t idx = (initial_idx + i) & MASK(map);
        sw_tlb_map_entry_t *ent = &map->entries[idx];
        if (!ent->valid) {
            ent->valid = 1;
            ent->vpn = vpn;
            ent->pfn = pfn;
            map->size++;
            return;
        }
    }
    // We must have had space in the map, so this should never happen
    assert(0);
}
