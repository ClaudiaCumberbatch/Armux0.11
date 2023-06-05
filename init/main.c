#include "bsp/uart.h"

void main(void)
{
  enable_uart0();
  // print_uart0("\r\nHello world!");
  printf("Hello %d %d\n", 1, 2);
  while (1)
    ; /*dont exit the program*/
}
