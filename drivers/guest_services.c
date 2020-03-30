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
 * @file guest_services.c
 * 
 * @section DESCRIPTION
 * 
 * This driver implements the guests hypercalls.
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
void get_vm_id(){
	MoveToPreviousGuestGPR(REG_A0,vcpu_in_execution->vm->id);
}

/**
 * @brief Send a message for a target VM. 
 * Calling convention (guest registers): 
 *   a0 = Target process ID
 *   a1 = Source message pointer 
 *   a2 = Message size 
 *   v0 = true/false (Succeded/failed)
 */
void intervm_send_msg(){
	vcpu_t* vcpu;
    

	/* Getting parameters from guest */
	uint32_t target_id  = MoveFromPreviousGuestGPR(REG_A0);
	char* message_ptr = (char*)MoveFromPreviousGuestGPR(REG_A1); 
	uint32_t message_size = MoveFromPreviousGuestGPR(REG_A2);
                        
	/* check if the message has acceptable size */
	if(message_size > MESSAGE_SZ){
		MoveToPreviousGuestGPR(REG_A0, MESSAGE_TOO_BIG);
		return;
	}
                        
	/* Try to locate the destiny VCPU */
	vcpu = (vcpu_t*)get_vcpu_from_id(target_id);

	/* destination vcpu not found */
	if(vcpu == NULL){
		MoveToPreviousGuestGPR(REG_A0, MESSAGE_VCPU_NOT_FOUND);
		return;
	}
                        
	if (vcpu->init){
		MoveToPreviousGuestGPR(REG_A0, MESSAGE_VCPU_NOT_INIT);
		return;
	}
                        
	/* message queue full */
	if(vcpu->messages.num_messages == MESSAGELIST_SZ){
		vcpu->guestclt2 |= (GUEST_INTERVM_INT<<GUESTCLT2_GRIPL_SHIFT);
		MoveToPreviousGuestGPR(REG_A0, MESSAGE_FULL);
		return;
	}   
     
	/* copy message to message queue */
	char* message_ptr_mapped = (char*)tlbCreateEntry((uint32_t)message_ptr, vm_in_execution->base_addr, message_size, 0xf, NONCACHEABLE);
	memcpy(vcpu->messages.message_list[vcpu->messages.in].message,message_ptr_mapped,message_size);
	vcpu->messages.message_list[vcpu->messages.in].size = message_size;
	vcpu->messages.message_list[vcpu->messages.in].source_id = vm_in_execution->id;
                        
	vcpu->messages.num_messages++;
	vcpu->messages.in = (vcpu->messages.in + 1) % MESSAGELIST_SZ; 
                        
	/* generate virtual interrupt to guest */
	//vcpu->guestclt2 |= (GUEST_INTERVM_INT<<VI_SHIFT);
                                
	/* Return success to sender */
	MoveToPreviousGuestGPR(REG_A0, message_size);
	
	//fast_interrupt_delivery(vcpu);
     
}

/**
 * @brief Receive a message. 
 * Calling convention (guest registers): 
 *   Input: 	a0 = Destination buffer.
 *   Output: 	v0 = Message size. 
 *   		a0 = Source ID.
 */
void intervm_recv_msg(){
	vcpu_t* vcpu = vcpu_in_execution;
	uint32_t messagesz;

	/* No messages in the receiver queue */
	if(vcpu->messages.num_messages == 0){
		MoveToPreviousGuestGPR(REG_A0, MESSAGE_EMPTY); 
		return;
	}

	/* Getting parameters from guest*/
	char* message_ptr = (char*)MoveFromPreviousGuestGPR(REG_A0);

	/* Copy the message the receiver */
	messagesz = vcpu->messages.message_list[vcpu->messages.out].size;
	char* message_ptr_mapped = (char*)tlbCreateEntry((uint32_t)message_ptr, vm_in_execution->base_addr, messagesz, 0xf, NONCACHEABLE);
	memcpy(message_ptr_mapped, vcpu->messages.message_list[vcpu->messages.out].message, messagesz);
    
	/* Return the message size to the receiver */
	MoveToPreviousGuestGPR(REG_A1, messagesz);
	MoveToPreviousGuestGPR(REG_A0, vcpu->messages.message_list[vcpu->messages.out].source_id); 
                        
	/* free the message allocation in the message list */
	vcpu->messages.num_messages--;
	vcpu->messages.out = (vcpu->messages.out + 1) % MESSAGELIST_SZ;
	
	/* clean interrupt */
	//setGuestCTL2(getGuestCTL2() & ~(GUEST_INTERVM_INT<<VI_SHIFT));
	//vcpu->guestclt2 &= ~(GUEST_INTERVM_INT<<VI_SHIFT);
                       
}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
void guest_services_init(){
	if (register_hypercall(get_vm_id, HCALL_GET_VM_ID) < 0){
		ERROR("Error registering the HCALL_GET_VM_ID hypercall");
		return;
	}

	if (register_hypercall(intervm_send_msg, HCALL_IPC_SEND_MSG) < 0){
		ERROR("Error registering the HCALL_IPC_SEND_MSG hypercall");
		return;
	}

	if (register_hypercall(intervm_recv_msg, HCALL_IPC_RECV_MSG) < 0){
		ERROR("Error registering the HCALL_IPC_RECV_MSG hypercall");
		return;
	}
    
	INFO("Inter-VM communication hypercalls registered.");
}

driver_init(guest_services_init);


