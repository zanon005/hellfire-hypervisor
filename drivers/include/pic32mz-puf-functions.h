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
 * @file pic32mz-puf-functions.h
 * 
 * @section DESCRIPTION
 * 
 * Specific functions for PUF access to flash and PUF generation. Required by Intrinsic-ID PUF implementation.
 */

#ifndef __PUF_FUNCTIONS_H
#define __PUF_FUNCTIONS_H


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

static uint32_t NVMUnlock(uint32_t nvmop);
static uint32_t NVMErasePage(void *address);
static uint32_t NVMWriteQuad(void *address, void *data);
static uint32_t flash_read1Kbuffer(uint8_t *buffer);
static uint32_t flash_write1Kbuffer(uint8_t *buffer);

#endif