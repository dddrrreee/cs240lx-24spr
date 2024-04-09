# Makefile to build or clean all labs.
SUBDIRS= 1-dynamic-code-gen 2-ir 3-ir-bootloader 4-ws2812b
# libunix libpi labs #  libunix

.PHONY: all check clean
all check clean: $(SUBDIRS)

all: TARGET=all
check: TARGET=check
clean: TARGET=clean

# No, you can't do TARGET=$@, or at least I don't know how to.

# recursive call to make
$(SUBDIRS): force
	$(MAKE) -s -C $@ $(TARGET) RUN=0

clean:
	rm -f *~ *.bak

.PHONY: force
	force :;
