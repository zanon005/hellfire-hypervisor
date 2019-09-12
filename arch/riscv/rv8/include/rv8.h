#ifndef _RV8_
#define _RV8_

#define UART_PBASE(a) *(volatile unsigned*) (0x40003000 + (a))

/* FIXME: Define correct size and addresses. */
#define UART2_BASE 0 
#define UART2_SIZE 0 

#define UART_REG_RBR  		UART_PBASE(0)
#define UART_REG_TBR		UART_PBASE(0)
#define UART_REG_IIR		UART_PBASE(2)
#define IIR_TX_RDY			2
#define IIR_RX_RDY			4

#define HTIF_TOHOST			0x40008000





#endif /* _RV8_ */
