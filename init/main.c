#include "bsp/uart.h"
#include "bsp/bsp_int.h"
void main(void)
{
  // int_init();
  enable_uart0();
  // print_uart0("\r\nHello world!");
  printf("Hello %d %d\n", 1, 2);
  while (1)
    ; /*dont exit the program*/
}
