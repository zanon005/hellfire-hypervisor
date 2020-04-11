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

/*************************************************************
 * Ping-Pong application - Inter-VM communication.
 * 
 * To execute the Ping-Pong set the CFG_FILE on the main 
 * Makefile to the sample-ping-pong.cfg configuration file.  
 * 
 * The ping-pong sample measures the inter-VM communication 
 * latency with different message sizes. 
 */

#include <arch.h>
#include <libc.h>
#include <guest_interrupts.h>
#include <hypercalls.h>
#include <platform.h>
#include <network.h>


uint32_t message_buffer[256];

int main(){
	uint32_t source,i;
	int32_t ret;

	memset(message_buffer, 0, sizeof(message_buffer));

	mdelay(1000);

	while(1){

		ret = ReceiveMessage(&source, message_buffer, sizeof(message_buffer), 1);
		
		printf("\nMessage %s from %d", message_buffer, source);

		mdelay(1000);

	}

}

