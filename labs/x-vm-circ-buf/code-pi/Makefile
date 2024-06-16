PROGS := $(wildcard tests/*-test.c)

COMMON_SRC += vm-circ-buf-init.c
COMMON_SRC += vm-circ-buf-ops.c

BOOTLOADER=my-install
RUN = 1
CAN_EMIT=1

include $(CS240LX_2024_PATH)/libpi/mk/Makefile.robust
