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


#define PAGESIZE 4096
#define VIRTUALBASE 0x80000000

extern uint64_t __pages_start;
static const uint8_t *page_Buffer = (uint8_t *)&__pages_start;



static uint64_t * get_next_page(){
	static uint32_t next_page = 0;
	uint64_t *page_addr = page_Buffer + next_page  * PAGESIZE;
	memset(page_addr, 0, PAGESIZE);
	
	next_page++;

	return page_addr;
}



/** Get a random tlb index  */
uint32_t tblGetRandomIndex(){
	return 0;
}

/** Write an entry to the tbl.
	@param index tlb index. The current entry on index will be lost. 
	@param entry the entry to be written.
 */
void tlbEntryWrite(vm_t* vm, struct tlbentry *entry){
	uint64_t *page_table, *inner_page_table;
	uint32_t addr, i, first_va, last_va, first_va_jump, last_va_jump;

	/* Get level 1 and 2 page tables. */
	page_table = get_next_page();
	inner_page_table = get_next_page();

	/* Keep the root page table address for context switches*/
	vm->root_page_table = page_table;

	addr = vm->base_addr;

	first_va = VIRTUALBASE;
	last_va = ((entry->entrylo1 - entry->entrylo0)*2)<<12 + VIRTUALBASE;
	first_va_jump = first_va>>30;
	last_va_jump = last_va>>30;

	for(i=first_va_jump;i<=(last_va_jump - first_va_jump);i++){
	
	page_table[i] |= 1;
	page_table[i] |= (((uint64_t)page_table + 0x1000 + (0x1*(i-first_va_jump))) >> 12) << 10;

	}

	/* Get level 3 page table */
	page_table = inner_page_table;
	inner_page_table = get_next_page();

	first_va_jump = (first_va<<2)>>23;
	last_va_jump = (last_va<<2)>>23;

	for(i=first_va_jump;i<=(last_va_jump - first_va_jump);i++){

	page_table[i] |= 1;
	page_table[i] |= (((uint64_t)page_table + 0x1000 + (0x1*(i-first_va_jump))) >> 12) << 10;

	}

	page_table = inner_page_table;

	first_va_jump = (first_va<<11)>>23;
	last_va_jump = (last_va<<11)>>23;
	
	for(i=first_va_jump;i<=(last_va_jump - first_va_jump);i++){

	page_table[i] |= 0xf;
	page_table[i] |= (addr + (0x1*(i-first_va_jump)))<< 10;

	}

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
