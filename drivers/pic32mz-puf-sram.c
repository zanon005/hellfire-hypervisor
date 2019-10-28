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
 * @file pic32mz-puf-sram.c
 * 
 * @section DESCRIPTION
 * 
 * Specific driver to generate a SRAM PUF.
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


#define XRORED_VALUE1 0xFFFFFFFF
#define XRORED_VALUE0 0x00000000

extern int32_t* _puf;


/**
 * @brief Generate Correction Bits for ECC Repetition Code. 
 */
static int32_t sram_puf_not_initialized(uint8_t *buffer){

	int32_t i,sram,correction_data,nominal_value;
	int32_t *sram_address;
	int32_t id = 0;
	
	sram_address = (int32_t *)(&_puf);


	for(i=0;i<32;i++){

		sram = *(sram_address);

		nominal_value = sram&0x1;
		sram = sram>>1;

		if(nominal_value==0)
			correction_data = sram^XRORED_VALUE0;
		else
			correction_data = sram^XRORED_VALUE1;
		

		((uint32_t *)buffer)[i] = correction_data;

		if(nominal_value==1)
			id = id|(1<<i);
		
		sram_address++;
	}

	flash_write1Kbuffer(buffer);

	return id;
}

/**
 * @brief Generate PUF when ECC Repetition Code was initialized. 
 */

static int32_t sram_puf_initialized(uint8_t *buffer){

	int32_t i,sram,correction_data,bits31;
	int32_t  bit_id = 0;
	int32_t  id = 0;
	int32_t *sram_address;

	sram_address = (int32_t *)(&_puf);

	for(i=0;i<32;i++){

		sram = *(sram_address)>>1;

		correction_data = ((uint32_t *)buffer)[i];

		bits31 = sram^correction_data;

		while (bits31) { 
      		
      		bits31 &= (bits31-1) ; 
      		bit_id++; 
    
    	}


    	if(bit_id>15)
    		bit_id=1;
    	else
    		bit_id=0;
		
		if(bit_id==1)
			id = id|(1<<i);

		sram_address++;
		
	}

	return id;

}

static void puf_id(){

	uint8_t *buffer;
	int32_t i;
	int32_t flash_status = 0;
	int32_t id = 0;
	buffer = malloc(sizeof(uint8_t)*1024);



	if(flash_read1Kbuffer(buffer)!=1024)
		ERROR("Error reading PUF Flash.");

	for(i=0;i<256;i++){

		if(((uint32_t *)buffer)[i]!=XRORED_VALUE1)
			flash_status = 1;
	}

	if(flash_status==0){
		
		id = sram_puf_not_initialized(buffer);

	}else{
		
		id = sram_puf_initialized(buffer);
	}

	MoveToPreviousGuestGPR(REG_V0, id);
}

/**
 * @brief Initialize driver registering the hypercall. 
 */
static void sram_puf_init(){

	puf_id();

	if (register_hypercall(puf_id, HCALL_GET_PUF_ID) < 0){
		ERROR("Error registering the HCALL_GET_PUF_ID hypercall.");
		return;
	}

	INFO("PUF ID enabled.");

}

driver_init(sram_puf_init);