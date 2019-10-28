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
 * This is a Driver that search denied hypercalls in each VM. When a denied hypercall is founded, the driver blocks the VM.
 */

#include <hypercall.h>
#include <globals.h>
#include <driver.h>
#include <hypercall_defines.h>
#include <libc.h>
#include <vm.h>
#include <malloc.h>
#include <config.h>
#include <tlb.h>
#include <scheduler.h>
#include <vcpu.h>
#include <queue.h>

#define HYP_ORIGIN 0x80000000
#define VM_ORIGIN 0x9d000000
#define FUNCT_BITS 0x3F
#define OPCODE_BITS 0xFC000000
#define HYPCALL_CODE 0x28
#define R_TYPE_OPCODE 0x10
#define CODE_BITS 0x1FF800

/**
 * @brief Block the VM with denied hypercalls. 
 */

void invalid_hypercall(int32_t vm){
	
	vcpu_t * vcpu = NULL;

	vcpu = queue_get(scheduler_info.vcpu_ready_list,vm);
	vcpu->state = VCPU_BLOCKED;

	INFO("Virtual Machine %s is blocked, denied hypercall founded.",vcpu->vm->vm_name);
}

/**
 * @brief Initialize driver scanning the VM's memory. 
 */
void denied_hypercalls_init(){
	
	int32_t i,j,k,blocked;
	int32_t* adress;
	int32_t mem_size,hcall_funct,code,opcode,adraux;
	vcpu_t * vcpu = NULL;


	for(i=0;i<NVMACHINES;i++){

		blocked = 0;
		adress = 0;
		vcpu = queue_get(scheduler_info.vcpu_ready_list,i);

		for(j=1;j<VMCONF[i].num_tlb_entries;j++){

			if(adress < VMCONF[i].tlb[j].entrylo0){
				adraux = VMCONF[i].tlb[j].entrylo0;
				
			}

		}
		
		adress = ((adraux<<12)+HYP_ORIGIN);
		mem_size = *adress - VM_ORIGIN;	

		for(j=0;j<mem_size;j=j+4){
	
			hcall_funct=(*adress)&FUNCT_BITS;
			opcode=((*adress)&OPCODE_BITS)>>26;

			if(hcall_funct==HYPCALL_CODE&&opcode==R_TYPE_OPCODE){

				code=((*adress)&CODE_BITS)>>11;	

				for(k=0;k<VMCONF[i].denied_hypercalls_sz;k++){
					
					if((code==VMCONF[i].denied_hypercalls[k])&&(blocked==0)){

						invalid_hypercall(i);
						blocked++;

					}

				}
					
			}

			adress++;

		}

	if(blocked==0)
		INFO("Virtual Machine %s is clean of denied hypercalls.",vcpu->vm->vm_name);

	}

}


driver_init(denied_hypercalls_init);

