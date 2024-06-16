#include <assert.h>

#include "vm-circ-buf.h"

int main(void) {

    vm_circ_buf_t buf = vm_circ_buf_create_assert();

    buf.window[0] = 240u;
    assert(buf.window[VM_CIRC_PAGESIZE] == 240u);
    buf.window[VM_CIRC_PAGESIZE] = 140u;
    assert(buf.window[0] == 140u);

    vm_circ_buf_destroy(&buf);
    return 0;
}
