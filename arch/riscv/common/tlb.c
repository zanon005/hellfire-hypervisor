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

#include <tlb.h>
#include <types.h>
#include <hypercall_defines.h>
#include <vcpu.h>
#include <globals.h>
#include <proc.h>
#include <hal.h>



/** Get a random tlb index  */
uint32_t tblGetRandomIndex(){
	return 0;
}

/** Write an entry to the tbl.
	@param index tlb index. The current entry on index will be lost. 
	@param entry the entry to be written.
 */
void tlbEntryWrite(struct tlbentry *entry){


	
}

/** Create a temporary tlb entry mapped to non cachable area.
 *  Uses the VM base address as virtual address to map the page.
 *    @param address hypercall address event
 *    @param baseaddr virtual machine base address
 *    @param size guest data size
 *    @param use_cache Determines if the page will be mapped to KSEG1 or KSEG2
 *    @return Mapped address. 
 */
uint32_t tlbCreateEntry(uint32_t address, uint32_t baseaddr, uint32_t size, uint32_t tlbindex, uint32_t use_cache){ 
    
	return 0;
}