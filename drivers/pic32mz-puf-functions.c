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
 * @file pic32mz-puf-functions.c
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

/* Uses the last page 16Kb of the flash to data persistance */
uint32_t* pageFlash = (uint32_t *)0xbd1fc000;

/**
 * @brief Unlock flash for write erase/write operations. 
 */
uint32_t NVMUnlock(uint32_t nvmop){
	uint32_t status;

	/*  Suspend or Disable all Interrupts */
	asm volatile ("di %0" : "=r" (status));
	
	/* Enable Flash Write/Erase Operations and Select
	 * Flash operation to perform */
	NVMCON = nvmop;
	udelay(10);
	
        /* Write Keys */
	NVMKEY = 0x0;
	NVMKEY = 0xAA996655;
	NVMKEY = 0x556699AA;
	
	/* Start the operation using the Set Register */
	NVMCONSET = 0x8000;
	
	/* Wait for operation to complete */
	while (NVMCON & 0x8000);
	
	/* Restore Interrupts */
	if (status & 0x00000001){
		asm volatile ("ei");
	}else{
		asm volatile ("di");
	}
	
	/* Disable NVM write enable */
	NVMCONCLR = 0x0004000;
	
	/* Return WRERR and LVDERR Error Status Bits */
	return (NVMCON & 0x3000);
}

/**
 * @brief Erase a flash page. 
 */
uint32_t NVMErasePage(void *address){
	uint32_t res;

	/* Set NVMADDR to the Start Address of page to erase */
	NVMADDR = ((uint32_t)address) & 0x1fffffff;
	
	/* Unlock and Erase Page */
	res = NVMUnlock(0x4004);

	return res;
}

/**
 * @brief Write four int size to flash.
 */
uint32_t NVMWriteQuad(void *address, void *data){
	uint32_t res;

	/* Set NVMADDR to Start Address of row to program */
	NVMADDR = ((uint32_t)address) & 0x1fffffff;
	NVMDATA0 = (*((uint32_t *)data));
	
	/* value written to */
	NVMDATA1 = (*((uint32_t *)data+1));
	
	/* value written to */
	NVMDATA2 = (*((uint32_t *)data+2));
	
	/*  value written to */
	NVMDATA3 = (*((uint32_t *)data+3));
        
	/*  Unlock and Write Quad */
	res = NVMUnlock(0x4002);

	return res;
}

/**
 * @brief Read 1K byte from flash.
 * @param buffer Pointer to the destination buffer. 
 * @return Fix value 1024. 
 */
uint32_t flash_read1Kbuffer(uint8_t *buffer){
	uint32_t i;
	for(i=0; i<256; i++){
		((uint32_t *)buffer)[i] = pageFlash[i];
	}
	return 1024;
}

/**
 * @brief Write 1K byte to flash.
 * @param buffer Pointer to the source buffer. 
 * @return Fix value 1024. 
 */
uint32_t flash_write1Kbuffer(uint8_t *buffer){
	uint32_t i;
	NVMErasePage(pageFlash);
	for(i=0; i<256; i+=4){
		NVMWriteQuad((pageFlash+i), ((uint32_t*)buffer)+i);    
	}
	return 1024;
}

