# list out the driver program source 
PROGS := hello.c
START = objs/small-start.o
LPI = 
MEMMAP = ./memmap.fixed

#LIB_POST = ../../1-mailboxes/code.staff/libgcc.a
# set RUN = 1 if you want the code to automatically run after building.
#RUN = 1
OPT_LEVEL = -Os -fno-zero-initialized-in-bss
OPT_LEVEL = -O2 -fno-zero-initialized-in-bss


DEPS = ./Makefile
include $(CS240LX_2024_PATH)/libpi/mk/Makefile.template-fixed

all:: code-hello.h

clean::
	rm -f small-start.o code-hello.h

code-hello.h: hello.bin
	make -C ../code-gen
	../code-gen/code-gen  hello.bin > code-hello.h
