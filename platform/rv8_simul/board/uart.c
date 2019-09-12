#include <config.h>
#include <types.h>
#include <rv8.h>
#include <uart.h>

/**
 * @file uart.c
 * 
 * @section DESCRIPTION
 * 
 * UART initialization and low level UART functions. 
 * 
 */

/** 
 * @brief Configures UART as stdout. 
 * @param baudrate_u2 UART baudrate.
 * @param baudrate_u6 not used.
 * @param sysclk System clock. 
 */
void init_uart(uint32_t baudrate_u2, uint32_t baudrate_u6, uint32_t sysclk){


}


/** 
 * @brief Write char to UART2. 
 * @param c Character to be writed. 
 */
void putchar(uint8_t c){   
	while( !(UART_REG_IIR & IIR_TX_RDY) );
	UART_REG_TBR = c;
}


/** 
 * @brief Block and wait for a character. 
 * @return Read character. 
 */
uint32_t getchar(void){
}

