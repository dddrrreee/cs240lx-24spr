# list out the driver program source 
PROGS := 1-hello.c
PROGS := 2-ping-pong-byte.c

COMMON_SRC = ir-put-get.c 

# you should be able to remove this when done.
STAFF_OBJS += staff-ir-put-get.o

# you should be able to drop in yours in libpi.
STAFF_OBJS += rpi-thread.o
STAFF_OBJS += rpi-thread-asm.o

CFLAGS += -I../small-prog

LIB_POST := $(CS240LX_2024_PATH)/lib/libgcc.a

# set RUN = 1 if you want the code to automatically run after building.
RUN = 1
OPT_LEVEL = -O2

include $(CS240LX_2024_PATH)/libpi/mk/Makefile.template-fixed
