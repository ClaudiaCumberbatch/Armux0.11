// #include <unistd.h>

// extern int vsprintf();

// static char printbuf[1024];

// #define UART0_BASE 0x00001000
// #define UART0_CTRL ((unsigned int *)(UART0_BASE + 0x02))      /* UART Control Register */
// #define UART0_TxRxFIFO0 ((unsigned int *)(UART0_BASE + 0x00)) /* UART FIFO             */

#define UART0_BASE 0xe0000000
#define UART0_CTRL ((unsigned int *)(UART0_BASE + 0x00))      /* UART Control Register */
#define UART0_TxRxFIFO0 ((unsigned int *)(UART0_BASE + 0x30)) /* UART FIFO             */

volatile unsigned int *const TxRxUART0 = UART0_TxRxFIFO0;
volatile unsigned int *const ctrlUART0 = UART0_CTRL;

#define XUARTPS_CR_TXEN (1 << 4)  /* Transmit enable */
#define XUARTPS_CR_RXEN (1 << 2)  /* Receive enable */
#define XUARTPS_CR_TXRES (1 << 1) /* Software reset for Tx data path */
#define XUARTPS_CR_RXRES (1 << 0) /* Software reset for Rx data path */

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

void main(void)
{
  enable_uart0();
  print_uart0("\r\nHello world!");
  while (1)
    ; /*dont exit the program*/
}

// #define __va_rounded_size(TYPE) \
// 	(((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

// #define va_start(AP, LASTARG) \
// 	(AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))

// void va_end (char*);

// static int printf(const char *fmt, ...)
// {
// 	char* args;
// 	int i;

// 	va_start(args, fmt);
// 	write(1,printbuf,i=vsprintf(printbuf, fmt, args));
// 	va_end(args);
// 	return i;
// }