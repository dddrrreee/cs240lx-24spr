PROGS := $(wildcard tests/*-test.c)

COMMON_SRC += tests/common.c
COMMON_SRC += sw-tlb-mmu-init.S
COMMON_SRC += sw-tlb-mmu-write.c
COMMON_SRC += sw-tlb-handler.c
COMMON_SRC += sw-tlb-map.c

BOOTLOADER=my-install
RUN = 1

include $(CS240LX_2024_PATH)/libpi/mk/Makefile.robust
