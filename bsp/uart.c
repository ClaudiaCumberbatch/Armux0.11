#include "bsp/uart.h"

volatile unsigned int *const TxRxUART0 = UART0_TxRxFIFO0;
volatile unsigned int *const ctrlUART0 = UART0_CTRL;

void enable_uart0()
{
  *ctrlUART0 = XUARTPS_CR_TXEN | XUARTPS_CR_RXEN | XUARTPS_CR_TXRES | XUARTPS_CR_RXRES;
}

void print_uart0(const char *s)
{
  while (*s != '\0')
  {                                  /* Loop until end of string */
    *TxRxUART0 = (unsigned int)(*s); /* Transmit char */
    s++;                             /* Next char */
  }
}