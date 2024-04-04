# this is the makefile template we use --- a bunch of stuff
# happens in the included makefile.  we will simplify, but
# for the moment, time-pressure.

# the driver programs: these all must have a notmain.
PROGS := hello.c test-interfile.c test-demand.c test-assert.c test-panic.c

# shared code linked with all driver programs.
COMMON_OBJS := foo.o

# define this if you need to give the device for your pi
TTYUSB = 
BOOTLOADER = pi-install

# uncomment if you want it to automatically run.
RUN=0
CAN_EMIT = 1

include $(CS240LX_2024_PATH)/libpi/mk/Makefile.template-v4
