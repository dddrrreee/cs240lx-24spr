PROGS = gpio-int.c

COMMON_SRC = interrupts-vec-asm.S

# if you need to override the install
BOOTLOADER = my-install

CFLAGS = -I../test-gen-fancy
OPT_LEVEL = -Ofast
OPT_LEVEL = -O3

# automatically run the code
RUN = 1

include $(CS240LX_2024_PATH)/libpi/mk/Makefile.template-fixed
