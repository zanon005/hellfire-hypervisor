#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <qemu_virt.h>



#define UART0_CTRL_ADDR(a) *(uint8_t*) (0x10000000 + (a))

#define UART_RBR		0x00  /* Receive Buffer Register */
#define UART_THR      	0x00  /* Transmit Hold Register */
#define UART_IER      	0x01  /* Interrupt Enable Register */
#define UART_DLL      	0x00  /* Divisor LSB (LCR_DLAB) */
#define UART_DLM      	0x01  /* Divisor MSB (LCR_DLAB) */
#define UART_FCR      	0x02  /* FIFO Control Register */
#define UART_LCR      	0x03  /* Line Control Register */
#define UART_MCR      	0x04  /* Modem Control Register */
#define UART_LSR      	0x05  /* Line Status Register */
#define UART_MSR      	0x06  /* Modem Status Register */
#define UART_SCR      	0x07  /* Scratch Register */

#define UART_LCR_DLAB 	0x80  /* Divisor Latch Bit */
#define UART_LCR_8BIT 	0x03  /* 8-bit */
#define UART_LCR_PODD 	0x08  /* Parity Odd */

#define UART_LSR_DA   	0x01  /* Data Available */
#define UART_LSR_OE   	0x02  /* Overrun Error */
#define UART_LSR_PE   	0x04  /* Parity Error */
#define UART_LSR_FE   	0x08  /* Framing Error */
#define UART_LSR_BI   	0x10  /* Break indicator */
#define UART_LSR_RE   	0x20  /* THR is empty */
#define UART_LSR_RI   	0x40  /* THR is empty and line is idle */
#define UART_LSR_EF   	0x80  /* Erroneous data in FIFO */

#endif 