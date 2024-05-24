## Lab: using single stepping to check interleaving.

We'll use single-stepping to write a concurrency checker that is tiny
but mighty.

In many tools, we'd want to run a single instruction and then get control
back for a tool.

For this lab you'll have to read the comments. Sorry :( 
  - check-interleave.h: has the the interface.
  - check-interleave.c: has a working version.
  - tests/ has tests. you should build the code so these work.

-----------------------------------------------------------------------
### Part 0: turn off single-step when `A()` calls `A_terminated()`

If you compile and run the code you'll notice the single-step handler
prints a ton of instructions even though `A()` is tiny.  The reason for
this is that when A() exits, it jumps to `A_terminated()`.  A simple
hack here is to change the single step handler to compare the `pc`
value against `A_terminated()` and if equal, turn single stepping off.

The result of this is that the test output should do down dramatically.

Easiest approach: change the makefile to only run
`tests/0-epsilon-test.c`.

-----------------------------------------------------------------------
### Part 1: do a single switch from A() to B()

-----------------------------------------------------------------------
### Part 2: add some interesting tests.

Easy mode: get some "lock-free" code from GPT or Claude and check it.
Should be able to find some bugs.

More interesting: check gcc's synchonization library code.  The cool thing
about single-step is that we work just as well on machine code. (Though
it's harder to debug!).

Other possible: 
 - Rewrite circular to use compiler memory barriers and look for bugs.
 - Extend the checker to look for bugs in interrupt handlers.

-----------------------------------------------------------------------
### Extensions

There's an enormous number of extensions:
  - Extend the code to do 2 switches, 3 switches, etc.

  - Make the error handling better.  E.g., on the unix side,
    use `addr2line` to convert the code addresses we ran on to line numbers.

  - Speed: to reduce the number of switches notice that A() can only
    effect B() if it does a store or synchronization instruction.  B()
    can only be effected it if does a load or synchonization instruction.
    If A() only writes to its copy of registers (e.g., an
    add, subtract, multiply) it can't affect B().  (And vice versa.)
    
    Thus we only need to switch on memory or synchornization operations.

  - Speed: the number of paths grows exponentially with the number of
    switches.  The standard way to handle this is to hash the memory
    state and if you get to the same program point with the same
    state, prune execution.  

    As a crude method you could side-step the need to know what memory
    locations are being read or written by hashing the register set.  
