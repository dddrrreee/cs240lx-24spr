//! \file sw-tlb-handler.h
//! \brief Exception handling for the software TLB
//! \author Ammar Ratnani <ammrat13@gmail.com>
//!
//! This file defines how to set up the exception handlers to set up the
//! software TLB. Specifically, the function `sw_tlb_handler_initialize` will
//! register exception handlers on prefetch and data aborts. The exception
//! handlers should provide the illusion of memory from 0x80000000 up to
//! 0xffffffff - i.e. in the upper half of the address space.

#pragma once

//! \brief Initialize the exception handlers for the software TLB
//!
//! This function is called after the heap has been initialized and the MMU has
//! been enabled. It is responsible for setting up the exception handlers and
//! allocating any structures they need to function.
//!
//! The exception handlers must not use any lockdown indicies before the one
//! passed in. This is because the first few lockdown indicies are used to map
//! the kernel and devices.
//!
//! \param sw_tlb_idx The first free lockdown index
void sw_tlb_handler_initialize(unsigned lockdown_start_idx);

//! \brief The number of exceptions that have occured
extern volatile unsigned sw_tlb_num_exceptions;
