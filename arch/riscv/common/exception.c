/*
Copyright (c) 2016, prpl Foundation

Permission to use, copy, modify, and/or distribute this software for any purpose with or without 
fee is hereby granted, provided that the above copyright notice and this permission notice appear 
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

This code was written by Carlos Moratelli at Embedded System Group (GSE) at PUCRS/Brazil.

*/

/**
 * @file exception.c
 * 
 * @section DESCRIPTION
 * 
 * General exception handler implementation.  
 * 
 */


#include <types.h>
#include <hal.h>
#include <config.h>
#include <exception.h>
#include <libc.h>
#include <globals.h>
#include <mips_cp0.h>
#include <proc.h>
#include <hypercall.h>
#include <board.h>
#include <scheduler.h>
#include <vcpu.h>
#include <timer.h>


uint32_t count = 0;
/**
 * @brief Guest exit exception handler. 
 * 
 */
static void guest_exit_exception(long cause, uint64_t epc){
	int32_t mepc,mstatus;
	mepc = read_csr(mepc);
	mstatus = read_csr(mstatus);

	switch (cause) {
		case IRQ_S_EXT:	
			hypercall_execution();
			break;
		default:
			WARNING("Stopping VM %d due to cause error %d .mepc:%xmstatus:%x\n", vcpu_in_execution->vm->id, cause,mepc,mstatus);
			vcpu_in_execution->state = VCPU_BLOCKED;
			break;
	}
	
	/* Advance to the next instruction. */
	vcpu_in_execution->pc = epc+4;

	setEPC(vcpu_in_execution->pc);	

	if(vcpu_in_execution->state == VCPU_BLOCKED){
		run_scheduler();
	}
	
	return 0;
}

/**
 * @brief General exception handler. 
 * 
 */
void general_exception_handler(long mcause, uint64_t mepc){
	uint32_t cause = get_field(mcause, MCAUSE_MASK);

	/* Interruption */
	if(MCAUSE_INT & mcause){
		switch(cause){
			case IRQ_M_TIMER:
				count++;
				if (count % 1000 == 0){
				printf("\n*\n");
				write_csr(mip,read_csr(mip)|0x2);
				}			
				timer_interrupt_handler();
				break;
		}
	}else{ /* Exceptions */ 
		guest_exit_exception(cause, mepc);
	}
}
