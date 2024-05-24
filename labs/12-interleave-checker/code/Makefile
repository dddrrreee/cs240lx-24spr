#PROGS := tests/5-circular-q-ok-test.c

PROGS := $(wildcard tests/[01234567]*-test.c)
PROGS := tests/1-epsilon-should-fail-test.c
PROGS := tests/3-circular-ok-test.c
PROGS := tests/4-trylock-ok-test.c
PROGS := $(wildcard tests/[023567]*-test.c)

PROGS := tests/2-increment-fail-test.c
PROGS := tests/0-epsilon-test.c

COMMON_SRC += check-interleave.c   

# trivial asm to invoke system calls
COMMON_SRC += syscall-invoke-asm.S

# code for exception forwarding.
COMMON_SRC += staff-full-except.c

# you can drop your versions in from 140e.
STAFF_OBJS += staff-full-except-asm.o
STAFF_OBJS += staff-switchto-asm.o
STAFF_OBJS += staff-single-step.o

BOOTLOADER=my-install
RUN = 1
CAN_EMIT=1


EXCLUDE ?= grep -v simple_boot
include $(CS240LX_2024_PATH)/libpi/mk/Makefile.robust
