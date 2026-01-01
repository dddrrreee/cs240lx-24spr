# Creating Circular Buffers with Virtual Memory

Virtual memory has the powerful ability to map any virtual page to any physcial
frame. We can use this ability to create a more ergonomic interface to a ring
buffer, and one that's more efficient to boot.

The key idea is to have two contiguous virtual pages be synonyms for the same
physical frame. We get some nice properties with this setup, for example:
  * We can just write off the end of the first virtual page, and the data will
    correctly be rerouted to the start of the backing store.
  * The data in the circular buffer always has a contiguous preimage in the
    virtual address space, meaning for instance we can `memcpy` it like any
    other data.
  * The read pointer can always be placed to the left of (or exactly at) the
    write pointer, meaning for instance you don't need to disambiguate between a
    full and an empty buffer.

Finally, it seems a lot of computer architecture optimizations expect sequential
accesses, such as loop vectorization and prefetching. Having contiguous data, at
least in the virtual address space, allows us to take advantage of those
optimizations.

This strategy is used in [GNU Radio][1], and it is mentioned on [Wikipedia][2],
though it does not seem to be well known.

## Part 1: Pi Implementation

Look over the interface in `code-pi/vm-circ-buf.h`. Then, implement the
functions in `code-pi/vm-circ-buf-init.c` and `code-pi/vm-circ-buf-ops.c`. For
`vm_circ_buf_init`, look over the `vm_map_sec` function, and the associated
virtual memory library functions.

## Part 2: Unix Implementation

It turns out that this same technique can be used on Unix systems with little
modification. The only change to the interface in `code-unix/vm-circ-buf.h` was
to the initialization functions, meaning your `vm-circ-buf-ops.c` file from the
pi-side will likely work as-is.

For allocating the circular buffer, take a look at `mmap` and `memfd_create`. If
you're not on Linux, try using `shm_open` instead of `memfd_create`. It might
also be helpful to look over GNU Radio's implementation under
`gnuradio-runtime/lib/vmcircbuf*`.

## Extension Ideas

* Build a [`heatshrink`][3] decompressor using this circular buffer, and compare
  its performance to a standard ring buffer
* Build a ring buffer that "leapfrogs" pages in the virtual address space to
  approximate an ideal FIFO while using only one page of memory
* Build a lock-free version of this structure so that multiple processes can use
  it

[1]:
    https://github.com/gnuradio/gnuradio/blob/main/gnuradio-runtime/lib/vmcircbuf.h
    "gr::vmcircbuf"
[2]:
    https://en.wikipedia.org/wiki/Circular_buffer
    "Circular Buffer"
[3]:
    https://github.com/atomicobject/heatshrink
    "atomicobject/heatshrink"
