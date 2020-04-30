
/**
 * @file uart_driver.c
 * 
 * @section DESCRIPTION
 * 
 * Interrupt-driven virtualized console driver that queues the guest's UART read/write calls.
 * 
 * The guest's call the HCALL_UART_SEND/HCALL_UART_RECV 
 * hypercalls being blocked to wait by UART tranfers to complete
 * when necessary. 
 * 
 */

#include <globals.h>
#include <hal.h>
#include <libc.h>
#include <proc.h>
#include <hypercall_defines.h>
#include <hypercall.h>
#include <driver.h>
#include <interrupts.h>
#include <platform.h>


/** 
 * @brief UART TX hypercall.
 * Write characters to the hardware queue and block the VCPU when
 * the queue is full and still there are characteres to tranfer. 
 */
static void send(){
	/*TODO: Implement interrupt-driven support to avoid to block the hypervisor for long periods. */
	
	uint32_t i = 0;
	char* str  = (char*)MoveFromPreviousGuestGPR(REG_A0);
	uint32_t size = MoveFromPreviousGuestGPR(REG_A1); 

	/* Map the guest's buffer. */
	//char* str_mapped = (char*)tlbCreateEntry((uint32_t)str, vm_in_execution->base_addr, size, 0xf, NONCACHEABLE);
	char* str_mapped = vm_in_execution->base_addr - 0x80000000 + str;
	
	for(i=0; i<size; i++){
    	while ((UART0_CTRL_ADDR(UART_LSR) & UART_LSR_RI) == 0);
    	UART0_CTRL_ADDR(UART_THR) = str_mapped[i];	
    }
	
	MoveToPreviousGuestGPR(REG_A0, size);
}

/**
 * @brief UART Driver init call. 
 */
static void uart_driver_init(){
	
	if (register_hypercall(send, HCALL_UART_SEND) < 0){
		ERROR("Error registering the HCALL_GET_VM_ID hypercall");
		return;
	}
	
	printf("\nUART driver enabled.");
}

driver_init(uart_driver_init);

