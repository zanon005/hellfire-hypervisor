/*
 * Copyright (c) 2016, prpl Foundation
 * 
 * Permission to use, copy, modify, and/or distribute this software for any purpose with or without 
 * fee is hereby granted, provided that the above copyright notice and this permission notice appear 
 * in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
 * FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * This code was written by Carlos Moratelli at Embedded System Group (GSE) at PUCRS/Brazil.
 * 
 */

/**
 * @file gpr_context.c
 * 
 * @section DESCRIPTION
 * 
 *
 */

#include <gpr_context.h>
#include <libc.h>
#include <vcpu.h>
#include <proc.h>

/** 
 * @brief Stack pointer defined at baikal.ld linker script. 
 */
extern long _stack;


void print_stack(){
	uint32_t i;
	printf("\n==========================");
	for(i=((unsigned long)(&_stack))-GPR_SIZE;  i<((unsigned long)(&_stack)); i+=sizeof(unsigned long*)){
		printf("\n0x%x", *(long*)i);
	}
	printf("\n==========================");
}

/** 
 * @brief Copy the GPR from vcpu to the stack. 
 * @param grp_p Pointer to the address where the gpr is saved. 
 */
void gpr_context_restore(long* gpr_p){
	uint32_t i, j;
	for(i=((unsigned long)(&_stack))-GPR_SIZE, j=0; i<((unsigned long)(&_stack)); i+=sizeof(unsigned long*), j++){
		*(long*)i = gpr_p[j]; 
	}
}


/** 
 * @brief Copy the GPR from stack to the vcpu. 
 * @param grp_p Pointer to the address where gpr will be saved. 
 */
void gpr_context_save(long* gpr_p){
	uint32_t i, j;
	for(i=((long)(&_stack))-GPR_SIZE, j=0; i<((long)(&_stack)); i+=sizeof(long*), j++){
		gpr_p[j] = ((long)*(long*)i); 
	}
}


/** 
 * @brief Move data to the previous guest GPR on stack.
 * @param reg GPR number.
 * @param value Value to write on the stack.
 */
void MoveToPreviousGuestGPR(long reg, uint64_t value){
	uint64_t* sp = (((uint64_t*)(&_stack)) - GPR_SIZE);
	sp[reg] = value;
}


/** 
 * @brief Copy data from the previous guest GPR on stack.
 * @param reg GPR number.
 * @return GPR data.
 */
uint64_t MoveFromPreviousGuestGPR(long reg){
	uint64_t* sp = (((uint64_t*)(&_stack)) - GPR_SIZE);
	return sp[reg];
}



