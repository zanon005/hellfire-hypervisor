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
 * @file hal.c
 * 
 * @section DESCRIPTION
 * 
 * Functions for access and configuration of the CP0 and Guest CP0. 
 * Supports the M5150 processor core and initial hypervisor configuration. 
 */

#include <libc.h>
#include <types.h>
#include <hal.h>
#include <globals.h>
#include <proc.h>
#include <board.h>
#include <malloc.h>
#include <vm.h>
#include <driver.h>
#include <timer.h>
#include <libc.h>

/* HEAP size as calculated by the linker script. */
extern uint32_t _heap_size;

/* Stringfy compiler parameters. */
#define STR(x) #x
#define STR_VALUE(x) STR(x)


/**
 * @brief Early boot message. 
 * 	Print to the stdout usefull hypervisor information.
 */
static void print_config(void)
{
	INFO("===========================================================");
	INFO("prplHypervisor %s [%s, %s]", STR_VALUE(HYPVERSION), __DATE__, __TIME__);
	INFO("Copyright (c) 2016, prpl Foundation");
	INFO("===========================================================");
	INFO("CPU Core:      %s", STR_VALUE(CPU_ID));
	INFO("Board:         %s", STR_VALUE(CPU_ARCH));
	INFO("System Clock:  %dMHz", CPU_FREQ/1000000);
	INFO("Heap Size:     %dKbytes", (int)(&_heap_size)/1024);
	INFO("Scheduler:     %dms", QUANTUM_SCHEDULER_MS);
	INFO("Guest Tick:    %dms", SYSTEM_TICK_US/1000);
	INFO("VMs:           %d\n", NVMACHINES);
}



/**
 * @brief C code entry. This is the first C code performed during 
 * hypervisor initialization. Called from early boot stage to perform 
 * overall hypervisor configuration. 
 * 
 * The hypervisor should never return form this call. The start_timer()
 * call should configure the system timer and wait by the first 
 * timer interrupt.
 * 
  */
void hyper_init(){
	
	/* Specific board configuration. */
	early_platform_init();    
	
	/* early boot messages with hypervisor configuration. */
	print_config();
	
	/* Processor inicialization */
	if(LowLevelProcInit()){
		CRITICAL("Low level processor initialization error.");
	}
	
	/* Configure the HEAP space on the allocator */ 
	init_mem();
	
	/*Initialize VCPUs and virtual machines*/
	initializeMachines();
	
	/* Initialize device drivers */    
	drivers_initialization();
	
	/* Start system timer. Should not return from this call.
	 *	   This call will wait for the first timer interrupt. */
	start_timer();
	
	/* Should never reach this point !!! */
	CRITICAL("Hypervisor initialization error.");
}


/**
 * @brief Verify if the processor is in root-kernel mode. 
 * @return 1 for root-kernel mode, 0 otherwise.
 */
int32_t isRootMode(){
	return 0;
}


/**
 * @brief Verify if the processor implements the VZ module . 
 * @return 1 VZ module available, 0 otherwise.
 */
int32_t hasVZ(){
	return 0;
}


/**
 * @brief Set the GPR Shadow Bank. 
 *      The hypervisor uses the GPR shadow page 0. The others GPR shadows keep the VM's context.
 * 
 * @return 0 for error or 1 for success.
 */
int32_t ConfigureGPRShadow(){
	return 0;
}


/**
 * @brief Low level processor initialization. 
 *      Called once during hypervisor Initialization.
 * 
 * @return 1 for error or 0 for success.
 */
int32_t LowLevelProcInit(){
    
	return 0;
}


/**
 * @brief Extract the execCode field from cause register
 * 
 * @return ExecCode value.
 */
uint32_t getCauseCode(){
	return 0;
}


/**
 * @brief Extract the hypercall code from hypercall instruction. 
 * 
 * @return Hypercall code.
 */
uint32_t getHypercallCode(){
	return 0;	
}


/**
 * @brief Checks if the exception happend on a branch delay slot. 
 * 
 * @return 0 for non branch delay slot, otherwise greater than 0.
 */
uint32_t getCauseBD(){
	return 0;
}


/**
 * @brief Extracts the  Guest ExecCode field from guestctl0 register. 
 * 
 * @return Guest ExecCode.
 */
uint32_t getGCauseCode(){
	return 0;
}


/**
 * @brief Extracts the interrupt pending bits (IP0:IP9) from cause register.
 * 
 * @return Interrupt pending bits.
 */
uint32_t getInterruptPending(){
	return 0;
}	


/**
 * @brief Set the Compare register to the next timer interruption.
 * 
 */
void confTimer(uint32_t quantum){
}


/**
 * @brief Set the IM bits on the status reg.
 * 
 */
void setInterruptMask(uint32_t im){
}


/**
 * @brief Clear the IM bits on the status reg .
 * 
 */
void clearInterruptMask(uint32_t im){
	
}


/**
 * @brief Enable global interrupt. IE bit in status register.
 * 
 */
void enableIE(){
	
}


/**
 * @brief Verify if the processor implements GuestID field.
 * 
 * @return 1 for guestid supported, 0 otherwise.
 */
uint32_t hasGuestID(){
	return 0;
}


/**
 * @brief Verify if the processor allow direct root mode.
 * 
 * @return 1 for direct root supported, 0 otherwise.
 */
uint32_t isDirectRoot(){
	return 0;
}


/**
 * @brief Set CP0 EPC. 
 * 
 */
void setEPC(uint32_t epc){
    
}


/**
 * @brief Get CP0 EPC. 
 * 
 * @return EPC address.
 */
uint32_t getEPC(){
	return 0;
}


/**
 * @brief Set root GuestID mode. 
 * 
 */
void setGuestRID(uint32_t guestrid){
}


/**
 * @brief Set GuestID. 
 * 
 */
void setGuestID(uint32_t guestid){
	
}


/**
 * @brief Get GuestID. 
 * 
 */
uint32_t getGuestID(void){
	return 0;
}


/**
 * @brief Check if the processor uses root ASID. 
 * 
 * @return 1 for root ASID supported, 0 otherwise
 */
uint32_t isRootASID(){
	return 0;
}


/**
 * @brief Set the processor's to Guest Mode bit. 
 * 
 */
void setGuestMode(){
	
}

/**
 * @brief Clear the processor's to Guest Mode bit. 
 * 
 */
void clearGuestMode(){
	
}


/**
 * @brief Set specific bits on CP0 STATUS reg.. 
 * 
 */
void setStatusReg(uint32_t bits){
	
}


/**
 * @brief Check if the processor supports 1k page size. 
 * 
 * @return 0 for not supported, greather than 0 for supported. 
 */
uint32_t has1KPageSupport(){
	return  0;
}


/**
 * @brief Disable 1K page support to keep compatibility with 4K page size. 
 * 
 */
void Disable1KPageSupport(){
	
}


/**
 * @brief Check if the processor will enter in guest mode on next eret instruction.
 * @return 1 is entering in guest mode, 0 no entering in guest mode. 
 */
int32_t isEnteringGuestMode(){
	return 0;
}


/**
 * @brief Get bad instruction address .
 * @return Bad instruction address. 
 */
uint32_t getBadVAddress(){
	return 0;
}

/**
 * @brief Set the Lowest GPR Shadow.
 * @return GPR value. 
 */
void setLowestGShadow(uint32_t lowestshadow){
	
}


/**
 * @brief Get the Lowest GPR Shadow.
 * @return Lowest guest GPR shadow. 
 */
uint32_t getLowestGShadow(void){
	return 0;
}


/**
 * @brief Set the previous shadow set.
 */
void setPreviousShadowSet(uint32_t shadow_set){
}

/**
 * @brief Get the previous  GPR Shadow.
 * @return Previous guest GPR shadow. 
 */
uint32_t getPreviousShadowSet(){
	return 0;
}

/**
 * @brief Get the number of GPR Shadows.
 * @return Number of GPR shadows. 
 */
uint32_t getNumberGPRShadow(){
	return 0;
}


/**
 * @brief Return the CP0 COUNTER.
 * @return CP0 COUNTER. 
 */
uint32_t getCounter(void){
	return 0;
}


/**
 * @brief Set CP0 GTOffset .
 */
void setGTOffset(int32_t gtoffset){
    
}


/**
 * @brief Set CP0 GuestCLT2 .
 */
void setGuestCTL2(uint32_t guestclt2){
    
}

/**
 * @brief Get CP0 GuestCLT2 .
 * 
 * @return guestctl2 register value.
 */
uint32_t getGuestCTL2(){
    return 0;
}


/**
 * @brief Get CP0 Random .
 * @return Random value.
 */
uint32_t getRandom(){
    return 0;
}

/**
 * @brief Get the most recent instruction which caused a exception.
 * @return Bad Instruction.
 */
uint32_t getBadInstruction(){
	return 0;
}


/**
 * @brief Wait for microseconds. 
 * @param usec Wait time.
 */
void udelay (uint32_t usec){

}

long read_csr(int csr){
    long result = -1;

    switch (csr) {
    case CSR_FFLAGS:     result = read_csr_m(0x001); break;
    case CSR_FRM:        result = read_csr_m(0x002); break;
    case CSR_FCSR:       result = read_csr_m(0x003); break;
    case CSR_MCYCLE:     result = read_csr_m(0xB00); break;
    case CSR_MINSTRET:   result = read_csr_m(0xB02); break;
    case CSR_MCYCLEH:    result = read_csr_m(0xB80); break;
    case CSR_MINSTRETH:  result = read_csr_m(0xB82); break;
    case CSR_CYCLE:      result = read_csr_m(0xC00); break;
    case CSR_TIME:       result = read_csr_m(0xC01); break;
    case CSR_INSTRET:    result = read_csr_m(0xC02); break;
    case CSR_CYCLEH:     result = read_csr_m(0xC80); break;
    case CSR_TIMEH:      result = read_csr_m(0xC81); break;
    case CSR_INSTRETH:   result = read_csr_m(0xC82); break;
    case CSR_MVENDORID:  result = read_csr_m(0xF11); break;
    case CSR_MARCHID:    result = read_csr_m(0xF12); break;
    case CSR_MIMPID:     result = read_csr_m(0xF13); break;
    case CSR_MHARTID:    result = read_csr_m(0xF14); break;
    case CSR_MSTATUS:    result = read_csr_m(0x300); break;
    case CSR_MISA:       result = read_csr_m(0x301); break;
    case CSR_MEDELEG:    result = read_csr_m(0x302); break;
    case CSR_MIDELEG:    result = read_csr_m(0x303); break;
    case CSR_MIE:        result = read_csr_m(0x304); break;
    case CSR_MTVEC:      result = read_csr_m(0x305); break;
    case CSR_MCOUNTEREN: result = read_csr_m(0x306); break;
    case CSR_MSCRATCH:   result = read_csr_m(0x340); break;
    case CSR_MEPC:       result = read_csr_m(0x341); break;
    case CSR_MCAUSE:     result = read_csr_m(0x342); break;
    case CSR_MTVAL:      result = read_csr_m(0x343); break;
    case CSR_MIP:        result = read_csr_m(0x344); break;
    case CSR_SSTATUS:    result = read_csr_m(0x100); break;
    case CSR_SEDELEG:    result = read_csr_m(0x102); break;
    case CSR_SIDELEG:    result = read_csr_m(0x103); break;
    case CSR_SIE:        result = read_csr_m(0x104); break;
    case CSR_STVEC:      result = read_csr_m(0x105); break;
    case CSR_SCOUNTEREN: result = read_csr_m(0x106); break;
    case CSR_SSCRATCH:   result = read_csr_m(0x140); break;
    case CSR_SEPC:       result = read_csr_m(0x141); break;
    case CSR_SCAUSE:     result = read_csr_m(0x142); break;
    case CSR_STVAL:      result = read_csr_m(0x143); break;
    case CSR_SIP:        result = read_csr_m(0x144); break;
    case CSR_SATP:       result = read_csr_m(0x180); break;
    case CSR_PMPCFG0:    result = read_csr_m(0x3A0); break;
    case CSR_PMPCFG1:    result = read_csr_m(0x3A1); break;
    case CSR_PMPCFG2:    result = read_csr_m(0x3A2); break;
    case CSR_PMPCFG3:    result = read_csr_m(0x3A3); break;
    case CSR_PMPADDR0:   result = read_csr_m(0x3B0); break;
    case CSR_PMPADDR1:   result = read_csr_m(0x3B1); break;
    case CSR_PMPADDR2:   result = read_csr_m(0x3B2); break;
    case CSR_PMPADDR3:   result = read_csr_m(0x3B3); break;
    case CSR_PMPADDR4:   result = read_csr_m(0x3B4); break;
    case CSR_PMPADDR5:   result = read_csr_m(0x3B5); break;
    case CSR_PMPADDR6:   result = read_csr_m(0x3B6); break;
    case CSR_PMPADDR7:   result = read_csr_m(0x3B7); break;
    case CSR_PMPADDR8:   result = read_csr_m(0x3B8); break;
    case CSR_PMPADDR9:   result = read_csr_m(0x3B9); break;
    case CSR_PMPADDR10:  result = read_csr_m(0x3BA); break;
    case CSR_PMPADDR11:  result = read_csr_m(0x3BB); break;
    case CSR_PMPADDR12:  result = read_csr_m(0x3BC); break;
    case CSR_PMPADDR13:  result = read_csr_m(0x3BD); break;
    case CSR_PMPADDR14:  result = read_csr_m(0x3BE); break;
    case CSR_PMPADDR15:  result = read_csr_m(0x3BF); break;
    default: 
    	CRITICAL("CSR not found.");
    	break;
    }
    return result;
}

void write_csr(int csr, long value){

    switch (csr) {
    case CSR_FFLAGS:     write_csr_m(0x001, value); break;
    case CSR_FRM:        write_csr_m(0x002, value); break;
    case CSR_FCSR:       write_csr_m(0x003, value); break;
    case CSR_MCYCLE:     write_csr_m(0xB00, value); break;
    case CSR_MINSTRET:   write_csr_m(0xB02, value); break;
    case CSR_MCYCLEH:    write_csr_m(0xB80, value); break;
    case CSR_MINSTRETH:  write_csr_m(0xB82, value); break;
    case CSR_CYCLE:      write_csr_m(0xC00, value); break;
    case CSR_TIME:       write_csr_m(0xC01, value); break;
    case CSR_INSTRET:    write_csr_m(0xC02, value); break;
    case CSR_CYCLEH:     write_csr_m(0xC80, value); break;
    case CSR_TIMEH:      write_csr_m(0xC81, value); break;
    case CSR_INSTRETH:   write_csr_m(0xC82, value); break;
    case CSR_MVENDORID:  write_csr_m(0xF11, value); break;
    case CSR_MARCHID:    write_csr_m(0xF12, value); break;
    case CSR_MIMPID:     write_csr_m(0xF13, value); break;
    case CSR_MHARTID:    write_csr_m(0xF14, value); break;
    case CSR_MSTATUS:    write_csr_m(0x300, value); break;
    case CSR_MISA:       write_csr_m(0x301, value); break;
    case CSR_MEDELEG:    write_csr_m(0x302, value); break;
    case CSR_MIDELEG:    write_csr_m(0x303, value); break;
    case CSR_MIE:        write_csr_m(0x304, value); break;
    case CSR_MTVEC:      write_csr_m(0x305, value); break;
    case CSR_MCOUNTEREN: write_csr_m(0x306, value); break;
    case CSR_MSCRATCH:   write_csr_m(0x340, value); break;
    case CSR_MEPC:       write_csr_m(0x341, value); break;
    case CSR_MCAUSE:     write_csr_m(0x342, value); break;
    case CSR_MTVAL:      write_csr_m(0x343, value); break;
    case CSR_MIP:        write_csr_m(0x344, value); break;
    case CSR_SSTATUS:    write_csr_m(0x100, value); break;
    case CSR_SEDELEG:    write_csr_m(0x102, value); break;
    case CSR_SIDELEG:    write_csr_m(0x103, value); break;
    case CSR_SIE:        write_csr_m(0x104, value); break;
    case CSR_STVEC:      write_csr_m(0x105, value); break;
    case CSR_SCOUNTEREN: write_csr_m(0x106, value); break;
    case CSR_SSCRATCH:   write_csr_m(0x140, value); break;
    case CSR_SEPC:       write_csr_m(0x141, value); break;
    case CSR_SCAUSE:     write_csr_m(0x142, value); break;
    case CSR_STVAL:      write_csr_m(0x143, value); break;
    case CSR_SIP:        write_csr_m(0x144, value); break;
    case CSR_SATP:       write_csr_m(0x180, value); break;
    case CSR_PMPCFG0:    write_csr_m(0x3A0, value); break;
    case CSR_PMPCFG1:    write_csr_m(0x3A1, value); break;
    case CSR_PMPCFG2:    write_csr_m(0x3A2, value); break;
    case CSR_PMPCFG3:    write_csr_m(0x3A3, value); break;
    case CSR_PMPADDR0:   write_csr_m(0x3B0, value); break;
    case CSR_PMPADDR1:   write_csr_m(0x3B1, value); break;
    case CSR_PMPADDR2:   write_csr_m(0x3B2, value); break;
    case CSR_PMPADDR3:   write_csr_m(0x3B3, value); break;
    case CSR_PMPADDR4:   write_csr_m(0x3B4, value); break;
    case CSR_PMPADDR5:   write_csr_m(0x3B5, value); break;
    case CSR_PMPADDR6:   write_csr_m(0x3B6, value); break;
    case CSR_PMPADDR7:   write_csr_m(0x3B7, value); break;
    case CSR_PMPADDR8:   write_csr_m(0x3B8, value); break;
    case CSR_PMPADDR9:   write_csr_m(0x3B9, value); break;
    case CSR_PMPADDR10:  write_csr_m(0x3BA, value); break;
    case CSR_PMPADDR11:  write_csr_m(0x3BB, value); break;
    case CSR_PMPADDR12:  write_csr_m(0x3BC, value); break;
    case CSR_PMPADDR13:  write_csr_m(0x3BD, value); break;
    case CSR_PMPADDR14:  write_csr_m(0x3BE, value); break;
    case CSR_PMPADDR15:  write_csr_m(0x3BF, value); break;
    default: 
		CRITICAL("CSR not found.");
    	break;
    }
}

void set_csr_bits(int csr, long bits){

    switch (csr) {
    case CSR_FFLAGS:     set_csr_m(0x001, bits); break;
    case CSR_FRM:        set_csr_m(0x002, bits); break;
    case CSR_FCSR:       set_csr_m(0x003, bits); break;
    case CSR_MCYCLE:     set_csr_m(0xB00, bits); break;
    case CSR_MINSTRET:   set_csr_m(0xB02, bits); break;
    case CSR_MCYCLEH:    set_csr_m(0xB80, bits); break;
    case CSR_MINSTRETH:  set_csr_m(0xB82, bits); break;
    case CSR_CYCLE:      set_csr_m(0xC00, bits); break;
    case CSR_TIME:       set_csr_m(0xC01, bits); break;
    case CSR_INSTRET:    set_csr_m(0xC02, bits); break;
    case CSR_CYCLEH:     set_csr_m(0xC80, bits); break;
    case CSR_TIMEH:      set_csr_m(0xC81, bits); break;
    case CSR_INSTRETH:   set_csr_m(0xC82, bits); break;
    case CSR_MVENDORID:  set_csr_m(0xF11, bits); break;
    case CSR_MARCHID:    set_csr_m(0xF12, bits); break;
    case CSR_MIMPID:     set_csr_m(0xF13, bits); break;
    case CSR_MHARTID:    set_csr_m(0xF14, bits); break;
    case CSR_MSTATUS:    set_csr_m(0x300, bits); break;
    case CSR_MISA:       set_csr_m(0x301, bits); break;
    case CSR_MEDELEG:    set_csr_m(0x302, bits); break;
    case CSR_MIDELEG:    set_csr_m(0x303, bits); break;
    case CSR_MIE:        set_csr_m(0x304, bits); break;
    case CSR_MTVEC:      set_csr_m(0x305, bits); break;
    case CSR_MCOUNTEREN: set_csr_m(0x306, bits); break;
    case CSR_MSCRATCH:   set_csr_m(0x340, bits); break;
    case CSR_MEPC:       set_csr_m(0x341, bits); break;
    case CSR_MCAUSE:     set_csr_m(0x342, bits); break;
    case CSR_MTVAL:      set_csr_m(0x343, bits); break;
    case CSR_MIP:        set_csr_m(0x344, bits); break;
    case CSR_SSTATUS:    set_csr_m(0x100, bits); break;
    case CSR_SEDELEG:    set_csr_m(0x102, bits); break;
    case CSR_SIDELEG:    set_csr_m(0x103, bits); break;
    case CSR_SIE:        set_csr_m(0x104, bits); break;
    case CSR_STVEC:      set_csr_m(0x105, bits); break;
    case CSR_SCOUNTEREN: set_csr_m(0x106, bits); break;
    case CSR_SSCRATCH:   set_csr_m(0x140, bits); break;
    case CSR_SEPC:       set_csr_m(0x141, bits); break;
    case CSR_SCAUSE:     set_csr_m(0x142, bits); break;
    case CSR_STVAL:      set_csr_m(0x143, bits); break;
    case CSR_SIP:        set_csr_m(0x144, bits); break;
    case CSR_SATP:       set_csr_m(0x180, bits); break;
    case CSR_PMPCFG0:    set_csr_m(0x3A0, bits); break;
    case CSR_PMPCFG1:    set_csr_m(0x3A1, bits); break;
    case CSR_PMPCFG2:    set_csr_m(0x3A2, bits); break;
    case CSR_PMPCFG3:    set_csr_m(0x3A3, bits); break;
    case CSR_PMPADDR0:   set_csr_m(0x3B0, bits); break;
    case CSR_PMPADDR1:   set_csr_m(0x3B1, bits); break;
    case CSR_PMPADDR2:   set_csr_m(0x3B2, bits); break;
    case CSR_PMPADDR3:   set_csr_m(0x3B3, bits); break;
    case CSR_PMPADDR4:   set_csr_m(0x3B4, bits); break;
    case CSR_PMPADDR5:   set_csr_m(0x3B5, bits); break;
    case CSR_PMPADDR6:   set_csr_m(0x3B6, bits); break;
    case CSR_PMPADDR7:   set_csr_m(0x3B7, bits); break;
    case CSR_PMPADDR8:   set_csr_m(0x3B8, bits); break;
    case CSR_PMPADDR9:   set_csr_m(0x3B9, bits); break;
    case CSR_PMPADDR10:  set_csr_m(0x3BA, bits); break;
    case CSR_PMPADDR11:  set_csr_m(0x3BB, bits); break;
    case CSR_PMPADDR12:  set_csr_m(0x3BC, bits); break;
    case CSR_PMPADDR13:  set_csr_m(0x3BD, bits); break;
    case CSR_PMPADDR14:  set_csr_m(0x3BE, bits); break;
    case CSR_PMPADDR15:  set_csr_m(0x3BF, bits); break;
    default: 
		CRITICAL("CSR not found.");
    	break;
    }
}

void clear_csr_bits(int csr, long bits){

    switch (csr) {
    case CSR_FFLAGS:     clear_csr_m(0x001, bits); break;
    case CSR_FRM:        clear_csr_m(0x002, bits); break;
    case CSR_FCSR:       clear_csr_m(0x003, bits); break;
    case CSR_MCYCLE:     clear_csr_m(0xB00, bits); break;
    case CSR_MINSTRET:   clear_csr_m(0xB02, bits); break;
    case CSR_MCYCLEH:    clear_csr_m(0xB80, bits); break;
    case CSR_MINSTRETH:  clear_csr_m(0xB82, bits); break;
    case CSR_CYCLE:      clear_csr_m(0xC00, bits); break;
    case CSR_TIME:       clear_csr_m(0xC01, bits); break;
    case CSR_INSTRET:    clear_csr_m(0xC02, bits); break;
    case CSR_CYCLEH:     clear_csr_m(0xC80, bits); break;
    case CSR_TIMEH:      clear_csr_m(0xC81, bits); break;
    case CSR_INSTRETH:   clear_csr_m(0xC82, bits); break;
    case CSR_MVENDORID:  clear_csr_m(0xF11, bits); break;
    case CSR_MARCHID:    clear_csr_m(0xF12, bits); break;
    case CSR_MIMPID:     clear_csr_m(0xF13, bits); break;
    case CSR_MHARTID:    clear_csr_m(0xF14, bits); break;
    case CSR_MSTATUS:    clear_csr_m(0x300, bits); break;
    case CSR_MISA:       clear_csr_m(0x301, bits); break;
    case CSR_MEDELEG:    clear_csr_m(0x302, bits); break;
    case CSR_MIDELEG:    clear_csr_m(0x303, bits); break;
    case CSR_MIE:        clear_csr_m(0x304, bits); break;
    case CSR_MTVEC:      clear_csr_m(0x305, bits); break;
    case CSR_MCOUNTEREN: clear_csr_m(0x306, bits); break;
    case CSR_MSCRATCH:   clear_csr_m(0x340, bits); break;
    case CSR_MEPC:       clear_csr_m(0x341, bits); break;
    case CSR_MCAUSE:     clear_csr_m(0x342, bits); break;
    case CSR_MTVAL:      clear_csr_m(0x343, bits); break;
    case CSR_MIP:        clear_csr_m(0x344, bits); break;
    case CSR_SSTATUS:    clear_csr_m(0x100, bits); break;
    case CSR_SEDELEG:    clear_csr_m(0x102, bits); break;
    case CSR_SIDELEG:    clear_csr_m(0x103, bits); break;
    case CSR_SIE:        clear_csr_m(0x104, bits); break;
    case CSR_STVEC:      clear_csr_m(0x105, bits); break;
    case CSR_SCOUNTEREN: clear_csr_m(0x106, bits); break;
    case CSR_SSCRATCH:   clear_csr_m(0x140, bits); break;
    case CSR_SEPC:       clear_csr_m(0x141, bits); break;
    case CSR_SCAUSE:     clear_csr_m(0x142, bits); break;
    case CSR_STVAL:      clear_csr_m(0x143, bits); break;
    case CSR_SIP:        clear_csr_m(0x144, bits); break;
    case CSR_SATP:       clear_csr_m(0x180, bits); break;
    case CSR_PMPCFG0:    clear_csr_m(0x3A0, bits); break;
    case CSR_PMPCFG1:    clear_csr_m(0x3A1, bits); break;
    case CSR_PMPCFG2:    clear_csr_m(0x3A2, bits); break;
    case CSR_PMPCFG3:    clear_csr_m(0x3A3, bits); break;
    case CSR_PMPADDR0:   clear_csr_m(0x3B0, bits); break;
    case CSR_PMPADDR1:   clear_csr_m(0x3B1, bits); break;
    case CSR_PMPADDR2:   clear_csr_m(0x3B2, bits); break;
    case CSR_PMPADDR3:   clear_csr_m(0x3B3, bits); break;
    case CSR_PMPADDR4:   clear_csr_m(0x3B4, bits); break;
    case CSR_PMPADDR5:   clear_csr_m(0x3B5, bits); break;
    case CSR_PMPADDR6:   clear_csr_m(0x3B6, bits); break;
    case CSR_PMPADDR7:   clear_csr_m(0x3B7, bits); break;
    case CSR_PMPADDR8:   clear_csr_m(0x3B8, bits); break;
    case CSR_PMPADDR9:   clear_csr_m(0x3B9, bits); break;
    case CSR_PMPADDR10:  clear_csr_m(0x3BA, bits); break;
    case CSR_PMPADDR11:  clear_csr_m(0x3BB, bits); break;
    case CSR_PMPADDR12:  clear_csr_m(0x3BC, bits); break;
    case CSR_PMPADDR13:  clear_csr_m(0x3BD, bits); break;
    case CSR_PMPADDR14:  clear_csr_m(0x3BE, bits); break;
    case CSR_PMPADDR15:  clear_csr_m(0x3BF, bits); break;
    default: 
		CRITICAL("CSR not found.");
    	break;
    }
}
