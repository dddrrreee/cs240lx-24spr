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

