/*
 * example using gpio interrupts.
 */
#include "rpi.h"
#include "timer-interrupt.h"
#include "cycle-count.h"

// loopback from 21 to 20.
enum { out_pin = 21, in_pin = 20 };

static volatile unsigned n_interrupt;

// when get to the interrupt.
static volatile uint32_t gpio_cycle;
// the value.
static volatile uint32_t gpio_val;

// called on gpio interrupts.
// currently cycle reg is garbage: you'll have to modify.
void int_vector(uint32_t pc, uint32_t cycle_reg) {
    unsigned s = cycle_cnt_read();
    gcc_mb();

    if(gpio_event_detected(in_pin)) {
        n_interrupt++;
        gpio_cycle = s;
        gpio_val = gpio_read(in_pin);
        gpio_event_clear(in_pin);
    }
}

// duplicate set_on/off so we can inline to reduce overhead.
// they have to run in < the delay we are shooting for.
#define GPIO_BASE 0x20200000
static inline void GPIO_SET_ON_RAW(unsigned pin) {
    volatile unsigned *gpio_set0  = (void*)(GPIO_BASE + 0x1C);
    *gpio_set0 = (1 << pin);
}
static inline void GPIO_SET_OFF_RAW(unsigned pin) {
    volatile unsigned *gpio_clr0  = (void*)(GPIO_BASE + 0x28);
    *gpio_clr0 = (1 << pin);
}

#include "asm-helpers.h"
cp_asm_set(vector_base_asm, p15, 0, c12, c0, 0)
cp_asm_get(vector_base_asm, p15, 0, c12, c0, 0)

void notmain() {
    extern uint32_t default_vec_ints[];

    // initialie interrupts to a clean state.
    dev_barrier();
    PUT32(Disable_IRQs_1, 0xffffffff);
    PUT32(Disable_IRQs_2, 0xffffffff);
    dev_barrier();
    vector_base_asm_set((uint32_t)default_vec_ints);
    
    void *exp = default_vec_ints;
    void *got = (void*)vector_base_asm_get();
    if(got != exp)
        panic("expected %p, have %p\n", got, exp);

    gpio_set_output(out_pin);
    gpio_set_input(in_pin);
    gpio_write(out_pin, 0);

#if 1
    // set interrupts on rising and falling edges.
    gpio_int_rising_edge(in_pin);
    gpio_int_falling_edge(in_pin);
#else
    gpio_int_async_rising_edge(in_pin);
    gpio_int_async_falling_edge(in_pin);
#endif

    // clear any existing event.
    gpio_event_clear(in_pin);
    system_enable_interrupts();
    caches_enable();


    if(gpio_read(in_pin))
        panic("should start w zero!\n");

    unsigned v = 0, c,e;
    
    // measure the overhead 
    for(int i = 0; i < 10; i++) {
        unsigned n = n_interrupt;
        output("n = %d: about to write\n", i);

        if(!v) {
            c = cycle_cnt_read();
            GPIO_SET_ON_RAW(out_pin);
        } else {
            c = cycle_cnt_read();
            GPIO_SET_OFF_RAW(out_pin);
        }
        do { 
            e = cycle_cnt_read();
        } while(n_interrupt == n);
        v = 1 - v;

        output("n = %d, time until read cycle=%d, time until return==%d\n", 
                i, 
                gpio_cycle - c,
                e - c);
    }
}
