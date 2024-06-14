#include "rpi.h"
#include "pt-vm.h"

void notmain(void) {

    // We're forced to use the heap because of `procmap`, so we'll put it
    // between 1MiB and 2MiB.
    kmalloc_init_set_start((void *)0x100000ul, 0x100000u);

    procmap_t pm = procmap_default_mk(1);
    vm_pt_t *pt = vm_map_kernel(&pm,0);
}
