#ifndef _UART_H
#define _UART_H

// #define UART0_BASE 0x00001000
// #define UART0_CTRL ((unsigned int *)(UART0_BASE + 0x02))      /* UART Control Register */
// #define UART0_TxRxFIFO0 ((unsigned int *)(UART0_BASE + 0x00)) /* UART FIFO             */

#define UART0_BASE 0xe0000000
#define UART0_CTRL ((unsigned int *)(UART0_BASE + 0x00))      /* UART Control Register */
#define UART0_TxRxFIFO0 ((unsigned int *)(UART0_BASE + 0x30)) /* UART FIFO             */


#define XUARTPS_CR_TXEN (1 << 4)  /* Transmit enable */
#define XUARTPS_CR_RXEN (1 << 2)  /* Receive enable */
#define XUARTPS_CR_TXRES (1 << 1) /* Software reset for Tx data path */
#define XUARTPS_CR_RXRES (1 << 0) /* Software reset for Rx data path */

void enable_uart0();
void print_uart0(const char *s);
void tfp_printf(char *fmt, ...);

#define printf tfp_printf 

#endif