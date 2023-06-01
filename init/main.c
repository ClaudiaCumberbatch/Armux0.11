// #include <unistd.h>

// extern int vsprintf();

// static char printbuf[1024];

#include "bsp/uart.h"

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