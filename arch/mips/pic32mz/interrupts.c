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
 * @file interrupts.c
 * 
 * @section DESCRIPTION
 * 
 * PIC32mz interrupt subsystem. Initializes the pic32mz interrupts
 * and allows for the registration of interrupt handlers. 
 * 
 * The hypervisor configures the pic32mz in vectored mode and uses 
 * the OFF register to configure the vector address of the interrupt. 
 * A device driver, after invoke register_interrupt() must configure
 * the corresponding OFF register with the function return.
 *
 */

#include<pic32mz.h>
#include <driver.h>
#include <mips_cp0.h>
#include <vcpu.h>
#include <config.h>
#include <globals.h>
#include <driver.h>
#include <interrupts.h>
#include <libc.h>

#define MAX_NUM_INTERRUPTS 10
#define VECTOR_1_OFFSET 0x220

/** 
 * @brief Interrupt handlers is an array of pointers to 
 * functions. Device drivers calls register_interrupt() to 
 * register a new interrupt handler. 
 */
handler_vector_t * interrupt_handlers[MAX_NUM_INTERRUPTS] = {NULL};

/** 
 * @brief Register a new interrupt handler. 
 * @param handler Function pointer. 
 * @return Relative interrupt address or 0 if the handler
 * could not be registered. 
 */
uint32_t register_interrupt(handler_vector_t * handler){
    uint32_t i;
    for(i=0; i<MAX_NUM_INTERRUPTS; i++){
        if(interrupt_handlers[i] == NULL){
            interrupt_handlers[i] = handler;
            return VECTOR_1_OFFSET + i*32;
        }
    }
    return 0;
}

/** 
 * @brief Configures the pic32mz in vectored interrupt mode. 
 * Additionally, configure all interrupt levels to be handled
 * at GPR shadow 0. 
 */
static void interrupt_init(){
    uint32_t temp_CP0;
    
    /* All root interrupt levels are handled at GPR shadow 0 where the
     * hypervisor resides.  Other GPR shadows are used for VM's execution. 
     */
    PRISS = 0;

    /* Configure the processor to vectored interrupt mode. */
    mtc0 (CP0_EBASE, 1, 0x9d000000);    /* Set an EBase value of 0x9D000000 */
    temp_CP0 = mfc0(CP0_CAUSE, 0);      /* Get Cause */
    temp_CP0 |= CAUSE_IV;               /* Set Cause IV */
    mtc0(CP0_CAUSE, 0, temp_CP0);       /* Update Cause */
    INTCONSET = INTCON_MVEC;            /* Set the MVEC bit - Vetored interrupt mode. */
    temp_CP0 = mfc0(CP0_STATUS, 0);     /* Get Status */
    temp_CP0 &= ~STATUS_BEV;            /* Clear Status IV */
    temp_CP0 &= ~STATUS_EXL; 
    mtc0(CP0_STATUS, 0, temp_CP0);      /* Update Status */
    
    mtc0(CP0_INTCTL, 1, mfc0(CP0_INTCTL, 1) | (INTCTL_VS << INTCTL_VS_SHIFT));
    
    INFO("PIC32mz in Vectored Interrupt Mode.");
}

driver_init(interrupt_init);

