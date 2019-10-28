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
 * @file pic32mz-puf-flash.c
 * 
 * @section DESCRIPTION
 * 
 * Specific driver for PUF access to flash. Required by Intrinsic-ID PUF implementation.
 */

#include <types.h>
#include <pic32mz.h>
#include <hal.h>
#include <globals.h>
#include <scheduler.h>
#include <hypercall_defines.h>
#include <hypercall.h>
#include <libc.h>
#include <driver.h>
#include <mips_cp0.h>
#include <globals.h>
#include <interrupts.h>
#include <pic32mz-puf-functions.h>


/**
 * @brief Hypercall to read 1K byte from flash. 
 * Calling convention (guest registers): 
 *   	Input: 	a0 = Destination buffer.
 * 	Output:	v0 = Number of bytes read or less than 0 for error. 
 */
static void flash_read(){
	uint32_t ret;
	
	uint8_t * dest = (uint8_t *) tlbCreateEntry((uint32_t) MoveFromPreviousGuestGPR(REG_A0), vm_in_execution->base_addr, sizeof(uint8_t) * 1024, 0xf, CACHEABLE);
	
	ret = flash_read1Kbuffer(dest);
	
	MoveToPreviousGuestGPR(REG_V0, ret);
}

/**
 * @brief Hypercall to write 1K byte to flash. 
 * Calling convention (guest registers): 
 *   	Input: 	a0 = Source buffer.
 * 	Output:	v0 = Number of bytes read or less than 0 for error. 
 */
static void flash_write(){
	uint32_t ret;
	
	uint8_t * source = (uint8_t *) tlbCreateEntry((uint32_t) MoveFromPreviousGuestGPR(REG_A0), vm_in_execution->base_addr, sizeof(uint8_t) * 1024, 0xf, CACHEABLE);
	
	ret = flash_write1Kbuffer(source);
	
	MoveToPreviousGuestGPR(REG_V0, ret);
}



/**
 * @brief Initialize driver registering the hypervcalls. 
 */
static void flash_puf_init(){
	if (register_hypercall(flash_read, HCALL_FLASH_READ) < 0){
		ERROR("Error registering the HCALL_FLASH_READ hypercall.");
		return;
	}
	
	if (register_hypercall(flash_write, HCALL_FLASH_WRITE) < 0){
		ERROR("Error registering the HCALL_FLASH_WRITE hypercall.");
		return;
	}
	
	INFO("Driver for PUF access to Flash enabled.")
}

driver_init(flash_puf_init);
