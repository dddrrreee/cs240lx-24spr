#include "rpi.h"

// do nothing: return's 0 so cstart
// zeroes bss.
int custom_c_runtime_init(void) {
    return 0;
}
