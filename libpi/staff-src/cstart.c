#include "rpi.h"
#include "cycle-count.h"
#include "memmap.h"
#include "redzone.h"

// called to setup the C runtime and initialize commonly
// used subsystems.
void _cstart() {
	void notmain();

    // if there is no customized c runtime 
    // initialization just zero out the bss.
    if(!custom_c_runtime_init()) {
        void * bss      = __bss_start__;
        void * bss_end  = __bss_end__;
        memset(bss, 0, bss_end-bss);
#if 0
        while( bss < bss_end )
            *bss++ = 0;
#endif
    }

    redzone_init();

    // currently default init to 115200 baud.
    uart_init();

    // have to initialize the cycle counter or it returns 0.
    // i don't think any downside to doing here.
    cycle_cnt_init();

    // should add argv.
    notmain(); 
	clean_reboot();
}
