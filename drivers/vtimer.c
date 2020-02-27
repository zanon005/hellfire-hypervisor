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
 * @file vtimer.c
 * 
 * @section DESCRIPTION
 * 
 * This driver register the mtimer hypercall.
 */

#include <hypercall.h>
#include <scheduler.h>
#include <hal.h>
#include <globals.h>
#include <tlb.h>
#include <driver.h>
#include <hypercall_defines.h>
#include <proc.h>
#include <guest_interrupts.h>
#include <libc.h>

/**
 * @brief Hypercall implementation. Returns the VM identifier number for the calling VM. 
 * V0 guest register will be replaced with the VM id. 
 */
void get_mtimer_value(){
	uint64_t mtime = MTIME;

	MoveToPreviousGuestGPR(REG_A0,mtime);
}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
void vtimer_init(){
	if (register_hypercall(get_mtimer_value, HCALL_GET_MTIMER_VALUE) < 0){
		ERROR("Error registering the HCALL_GET_MTIMER_VALUE hypercall");
		return;
	}
    
	INFO("Hypercall MTimer Value implemented.");
}

driver_init(vtimer_init);



