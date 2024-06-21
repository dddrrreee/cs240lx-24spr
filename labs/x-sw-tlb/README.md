# Software-Managed TLB

It seems that some processors forego having a hardware page-table walker.
Instead, a TLB miss causes an exception, and it's the operating system's
responsibility to populate the relevant TLB entries so that hardware can use it.
This is termed a "[Software-Managed TLB][1]".

While our processor has a hardware page-table walker, there's an option to
disable it in the Translation Table Base Control Register (DDI 0301H § 3.2.15).
As a result, we can create a software-managed TLB using the lockdown region,
just as we did in CS 140E [Lab 15][2].

## Task Description

Your task is to provide the illusion that memory exists in the upper half of the
address space - from `0x80000000` up to `0xffffffff` (inclusive). You can do
this by assuming that programs will never try to use all of that address space
at once, so you can dynamically allocate pages "on the fly" as software reads
and writes them.

To do this, you must write the `sw_tlb_handler_initialize` method in
`sw-tlb-handler.c`. That function should install exception handlers for data and
prefetch aborts. It can also allocate any necessary metadata for the exception
handlers.

The handlers themselves should find what virtual address faulted, figure out how
to map it into physical memory, then install the mapping in the TLB. You can do
this however you want, subject to the following constraints:
  * You must preserve true memory dependencies. If I write data to an address,
    then read it back, I should read what I wrote.
  * All pages must be zero-initialized. Reading from this "fake" memory without
    having written to it previously should return zero.
  * You must be able to have at least two mappings in the software-managed TLB
    at one time - one for instructions and one for data.
  * All large structures must be allocated on the heap with `kmalloc` or its
    variants. You have 15MiB to work with.

Consider the runtime of your exception handlers. They should be short, and
ideally operate in constant time. They must be sub-linear - their runtime cannot
scale as `O(m)` where `m` is the memory usage of the application. Also consider
the page size to use. The tests for this lab access data in 4KiB chunks, so a
1MiB page size might cause too much internal fragmentation, especially since you
only have 15MiB of physical memory to work with. Finally consider how many times
your exception handlers need to be called. Try to keep entries in the TLB so you
don't have to repopulate them as often.

## Approaches

You can emulate the radix-tree structure used by hardware. But, I initially
designed this lab to teach [Inverted Page Tables][3]. The key idea is that, if
we don't care where exactly a virtual page gets placed in physical memory, we
can have the mapping defined by a hash function on the virtual page number. This
skips having to store the mapping explicitly, which either requires a lot of
memory or a lot of levels, especially with 64-bit address spaces. To deal with
collisions, you just store the reverse mapping in a frame table, and check
whether the virtual page number currently occupying the physical frame is the
one you're trying to use. If there's a mismatch, you can use some probing
technique, like linear probing or cuckoo hashing, to find the correct slot.

This is actually *not* what I implemented in my solution. I didn't like the
memory use for applications that don't use the whole physical address space, and
I didn't like that the frame table can't easily be resized to accomodate
different loads. Instead, I use use a [hashmap][4] to store the existing
mappings from virtual page numbers to physical frame numbers, and I allocate
physical frames as they're needed.

But anything goes as long as it fits the requirements.

## Possible Extensions
* Experiment with different eviction policies. I used round-robin, but check to
  see if random eviction works better. Also try to get LRU working.
* Experiment with different load factors if you used an inverted page table or a
  hash map. See which one results in the fewest probes.
* Experiment with different page sizes if you used a radix tree. Measure the
  fragmentation for each of them.
* Implement a [zero page][5]. Initially, redirect all reads to a single physical
  frame populated entirely with zeros. Only allocate a separate frame when the
  application actually writes to it.
* Port your allocation code to use the hardware page-table walker. With that,
  the point of the lab would more so shift to frame allocation.

[1]:
    https://en.wikipedia.org/wiki/Translation_lookaside_buffer#TLB-miss_handling
    "Translation Lookaside Buffer - TLB-Miss Handling"
[2]:
    https://github.com/dddrrreee/cs140e-24win/tree/main/labs/15-pinned-vm
    "15: Pinned VM"
[3]:
    https://cs.stackexchange.com/questions/84764/difference-between-inverted-page-table-and-a-standard-one
    "Difference between inverted page table and a standard one?"
[4]:
    https://www.youtube.com/watch?v=kVgy1GSDHG8
    "Coding Interviews Be Like"
[5]:
    https://stackoverflow.com/a/35550384
    "Linux kernel: Role of zero page allocation at paging_init time"
