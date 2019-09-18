#include <config.h>
#include <types.h>
#include <qemu_virt.h>
#include <uart.h>
#include <platform.h>

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
    uint32_t divisor = sysclk / (16 * baudrate_u2);

    UART0_CTRL_ADDR(UART_LCR) = UART_LCR_DLAB;
    UART0_CTRL_ADDR(UART_DLL) = divisor & 0xff;
    UART0_CTRL_ADDR(UART_DLM) = (divisor >> 8) & 0xff;
    UART0_CTRL_ADDR(UART_LCR) = UART_LCR_PODD | UART_LCR_8BIT;
}


/** 
 * @brief Write char to UART. 
 * @param c Character to be writed. 
 */
void putchar(uint8_t c){   
    while ((UART0_CTRL_ADDR(UART_LSR) & UART_LSR_RI) == 0);
    UART0_CTRL_ADDR(UART_THR) = c;
}


/** 
 * @brief Block and wait for a character. 
 * @return Read character. 
 */
uint32_t getchar(void){
}

